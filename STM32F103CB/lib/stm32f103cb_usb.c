/*!**************************************************************************
    @file stm32f103cb_usb.c
    @brief Source file for stm32f103cb USB
    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug 

    @details

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)
******************************************************************************/
#include "stm32f103cb_usb.h"

//Circular buffer for trasmitted data
#define USB_FIFO_SIZE 64
static uint8_t txbuffer[USB_FIFO_SIZE];
static uint8_t head;
static uint8_t tail;
static const uint8_t mask = USB_FIFO_SIZE-1;

/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[128];

//usb device
static usbd_device *usbd_dev;

//Received data function pointer
static void (*data_handler)(uint8_t byte);

static void cdcacm_data_rx_cb(usbd_device *usbd_dev_dum, uint8_t ep);

static const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_CDC,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x0483,
	.idProduct = 0x5740,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

static const struct usb_endpoint_descriptor comm_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x83,
	.bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
	.wMaxPacketSize = 16,
	.bInterval = 255,
}};

static const struct usb_endpoint_descriptor data_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x01,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 1,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = 0x82,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 1,
}};

static const struct {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_call_management_descriptor call_mgmt;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) cdcacm_functional_descriptors = {
	.header = {
		.bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_HEADER,
		.bcdCDC = 0x0110,
	},
	.call_mgmt = {
		.bFunctionLength =
			sizeof(struct usb_cdc_call_management_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
		.bmCapabilities = 0,
		.bDataInterface = 1,
	},
	.acm = {
		.bFunctionLength = sizeof(struct usb_cdc_acm_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_ACM,
		.bmCapabilities = 0,
	},
	.cdc_union = {
		.bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
		.bDescriptorType = CS_INTERFACE,
		.bDescriptorSubtype = USB_CDC_TYPE_UNION,
		.bControlInterface = 0,
		.bSubordinateInterface0 = 1,
	 },
};

static const struct usb_interface_descriptor comm_iface[] = {{
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 1,
	.bInterfaceClass = USB_CLASS_CDC,
	.bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
	.bInterfaceProtocol = USB_CDC_PROTOCOL_AT,
	.iInterface = 0,

	.endpoint = comm_endp,

	.extra = &cdcacm_functional_descriptors,
	.extralen = sizeof(cdcacm_functional_descriptors),
}};

static const struct usb_interface_descriptor data_iface[] = {{
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 1,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_DATA,
	.bInterfaceSubClass = 0,
	.bInterfaceProtocol = 0,
	.iInterface = 0,

	.endpoint = data_endp,
}};

static const struct usb_interface ifaces[] = {{
	.num_altsetting = 1,
	.altsetting = comm_iface,
}, {
	.num_altsetting = 1,
	.altsetting = data_iface,
}};

static const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 2,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0x32,

	.interface = ifaces,
};

static const char *usb_strings[] = {
	"Glob_lib",
	"USB",
	"BASIC",
};


static int cdcacm_control_request(usbd_device *usbd_dev_dum, 
        struct usb_setup_data *req, uint8_t **buf,
		uint16_t *len, void (**complete)(usbd_device *usbd_dev, 
        struct usb_setup_data *req)){

	(void)complete;
	(void)buf;
	(void)usbd_dev_dum;

	switch (req->bRequest) {
	case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {

		char local_buf[10];
		struct usb_cdc_notification *notif = (void *)local_buf;

		/* We echo signals back to host as notification. */
		notif->bmRequestType = 0xA1;
		notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
		notif->wValue = 0;
		notif->wIndex = 0;
		notif->wLength = 2;
		local_buf[8] = req->wValue & 3;
		local_buf[9] = 0;
		// usbd_ep_write_packet(0x83, buf, 10);
		return 1;
		}

	case USB_CDC_REQ_SET_LINE_CODING:
		if (*len < sizeof(struct usb_cdc_line_coding))
			return 0;
		return 1;
	}
	return 0;
}


static void cdcacm_set_config(usbd_device *usbd_dev_dum, uint16_t wValue)
{
	(void)wValue;
	(void)usbd_dev_dum;

	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL);
	usbd_ep_setup(usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL);

	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
				USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				(usbd_control_callback)cdcacm_control_request);
}

//This is called when data is received
static void cdcacm_data_rx_cb(usbd_device *usbd_dev_dum, uint8_t ep)
{
	(void)ep;
	(void)usbd_dev_dum;

    uint8_t buffer[64];
    static uint8_t flashPos = 0;
    static volatile uint8_t last;
    uint8_t length;

	length = usbd_ep_read_packet(usbd_dev, 0x01, buffer, 64);

    for(uint8_t i = 0; i < length; i++){

        //This conditional statement checks if 8 NULL characters have been
        //sent through USB.
        //This signals a reset to bootloader and is used to trigger a software
        //reprogram
        if((buffer[i] == '\0') && (last == '\0')){

            flashPos++;

            if(flashPos > 8){

                rcc_periph_clock_enable(RCC_BKP);
                rcc_periph_clock_enable(RCC_PWR);
                pwr_disable_backup_domain_write_protect();
                BKP_DR1 = 1234;
                pwr_enable_backup_domain_write_protect();
                USB_disconnect();
                POWER_reset();
            }

        }
        else{

            flashPos = 0;
        }

        //Check f_pointer is valid
        if(data_handler != NULL){

            data_handler(buffer[i]);
        }
        last = buffer[i];
    }
}

void USB_disconnect(void){

    //Disable the USB peripheral
    *((uint32_t*)( 0x40005C40)) |= 0x03;

    //Pull both lines low to tell the host the device is disonnected
    pinSetup(GPIO_DO,PORTA,PIN11);
    pinSetup(GPIO_DO,PORTA,PIN12);
    pinLow(PORTA,PIN11);
    pinLow(PORTA,PIN12);

    //Delay for host to recognise change
    for(volatile int i = 0; i < 120000;i++);
}


void USB_setup(void){

    USB_disconnect();
    data_handler = NULL;
	usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, 
                &dev, 
                &config, 
                usb_strings, 
                3, 
                usbd_control_buffer, 
                sizeof(usbd_control_buffer));
	            usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);


    //Need to have this here to handle initial setup of USB
    for(volatile int i = 0; i < 10000; i++){

        usbd_poll(usbd_dev);
    }
    
}

void USB_setGet(void (*data_available)(uint8_t)){

    //Set the handler for available data
    data_handler = data_available;
}

//This has two functions
//  1. It handles control requests from the host
//  2. It sends all data waiting in the transmit buffer
void USB_update(void){

    uint8_t toSend[USB_FIFO_SIZE];
    uint8_t pos = 0;
    
    while(head != tail){

        toSend[pos++] = txbuffer[tail];
        tail++;
        tail = (tail & mask);
    }

    if(pos){

         toSend[pos] = 0;
         usbd_ep_write_packet(usbd_dev, 0x82, toSend, pos);
    }

    usbd_poll(usbd_dev);
}

//This adds a byte to the transmit buffer
void USB_put(uint8_t byte){

    txbuffer[head] = byte;
    head++;
    head = (head & mask);
}
