#ifndef BOOT_H
#define BOOT_H_

/**
    @addtogroup USB_bootloader 
    @brief USB_bootloader for the STM32F103 microcontroller.
    @details 

      The USB bootloader is designed to upload new firmware on STM32F1 devices.

      The bootloader can always be entered by:

        - Holding Port A pin 1 high and cycle the power to the device.
        - Sending 8 null characters ( \0 ) sequentially through the USB port.
        - An occurance of a hard fault 
        - Reset from independant watchdog timer.

        These events are triggered by the value stored in the first data register
        of the backup domain. If this register is used for another function the 
        bootloader may not trigger from software as expected.

        The majority of this bootloader is adapted from libopencm3 examples.
        Modifications have been made to trigger the events above.

    @author Stuart Ianna
    @version 0.1
    @date June 2018
    @copyright GNU GPLv3
    @warning None
    @bug At some instances, the power to the MCU needs to be reset after new 
    firmware is loaded.
    @todo

    @par Compilers
    - arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

 * @{
 */

/*! @file usb_bootloader.h
    @brief Header file for the stm32f103 USB bootloader.
*/

#include <stm32f103cb_core.h>
#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/dfu.h>
#include <libopencm3/stm32/f1/bkp.h>
#include <libopencm3/stm32/f1/pwr.h>

/*! 
    @brief Offset for the application starting address.
    @details This would only need to be changed if the bootloader somehow got larger.
*/
#define APP_ADDRESS	0x08003000

/*! 
    @brief Command sent with wBlockNum == 0 as per ST implementation. 
*/
#define CMD_SETADDR	0x21
/*! 
    @brief Command sent with wBlockNum == 0 as per ST implementation. 
*/
#define CMD_ERASE	0x41

/*! 
    @brief Control buffer for device commands, 1024 bytes set by USB standard
*/
uint8_t usbd_control_buffer[1024];

/*! 
    @brief DFU idle state
*/
enum dfu_state usbdfu_state = STATE_DFU_IDLE;

/*! 
    @brief Buffer to store a single command sent from host.
*/
struct{
	uint8_t buf[sizeof(usbd_control_buffer)];
	uint16_t len;
	uint32_t addr;
	uint16_t blocknum;
}prog;

/*! 
    @brief USB device descriptor.
*/
const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x0483,
	.idProduct = 0xDF11,
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

/*! 
    @brief USB DFU descriptor.
*/
const struct usb_dfu_descriptor dfu_function = {
	.bLength = sizeof(struct usb_dfu_descriptor),
	.bDescriptorType = DFU_FUNCTIONAL,
	.bmAttributes = USB_DFU_CAN_DOWNLOAD | USB_DFU_WILL_DETACH,
	.wDetachTimeout = 255,
	.wTransferSize = 1024,
	.bcdDFUVersion = 0x011A,
};

/*! 
    @brief USB Interface descriptor.
*/
const struct usb_interface_descriptor iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints = 0,
	.bInterfaceClass = 0xFE, /* Device Firmware Upgrade */
	.bInterfaceSubClass = 1,
	.bInterfaceProtocol = 2,

	/* The ST Microelectronics DfuSe application needs this string.
	 * The format isn't documented... */
	.iInterface = 4,

	.extra = &dfu_function,
	.extralen = sizeof(dfu_function),
};

/*! 
    @brief USB Interfaces.
*/
const struct usb_interface ifaces[] = {{
	.num_altsetting = 1,
	.altsetting = &iface,
}};

/*! 
    @brief USB configuration.
*/
const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0xC0,
	.bMaxPower = 0x32,
	.interface = ifaces,
};

/*! 
    @brief USB description strings.
*/
const char *usb_strings[] = {
	"STM32F1 USB DFU Device",
	"PA1 High on power to enter bootloader",
	"V1.0",
	/* This string is used by ST Microelectronics' DfuSe utility. */
	"@Internal Flash   /0x08000000/8*001Ka,56*001Kg",
};

/*! 
    @brief Get USB Status
    @param usbd_dev This parameter is not used.
    @param bwPollTimeout Timeout time.
    @return DFU device status. This function allways return DFU_STATUS_OK.
*/
uint8_t usbdfu_getstatus(usbd_device *usbd_dev, uint32_t *bwPollTimeout);

/*! 
    @brief Burn the program to the flash memory.
    @details This function is called when new information is received from the host.
    It has two functions:
        1. Data is programmed to the flash memory, based on the set address and
        erase state.
        2.  When all data is programmed, the program disables all peripherals, writes
        1 to the backup domain so on reset the bootloader knows a program is present
        on reset.
    @param usbd_dev This parameter is not used.
    @param req This parameter is not used.
*/
void usbdfu_getstatus_complete(usbd_device *usbd_dev, struct usb_setup_data *req);

/*! 
    @brief Handle control requests from the host.
    @param usbd_dev Pointer to the usb device
    @param req USB setup data
    @param buf Buffer to store sent data.
    @param len Length of the data buffer.
    @param complete Funtion pointer to handler for USB command.
    @return 
        - 1 if valid control command was received. 
        - 0 if invalid control command was received. 
*/
int usbdfu_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf,uint16_t *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req));

/*! 
    @brief Set the USB configuration.
    @param usbd_dev Pointer to the usb device
    @param wValue This parameter is not used.
*/
void usbdfu_set_config(usbd_device *usbd_dev, uint16_t wValue);

/**@}*/
#endif
