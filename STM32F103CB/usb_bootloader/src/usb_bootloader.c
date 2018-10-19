#include "usb_bootloader.h"

int main(void)
{
	usbd_device *usbd_dev;
	rcc_periph_clock_enable(RCC_GPIOA);

    //Enable clocks so backup domain can be checked
    rcc_periph_clock_enable(RCC_BKP);
    rcc_periph_clock_enable(RCC_PWR);

    //This is written in main application on reset to bootloader request
    if(BKP_DR1 == 1234){

        //This will continue to load bootloader
    }
    else if(BKP_DR1 == 5678){

        //Reset from hard fault, start the bootloader
        //Clear this flag so a subsequent reset continues to application
        pwr_disable_backup_domain_write_protect();
        BKP_DR1 = 1;
        pwr_enable_backup_domain_write_protect();
    }
    else if (!gpio_get(GPIOA, GPIO0)) {

        //Write 1 so that applicaiton knows the bootloader is used.
        pwr_disable_backup_domain_write_protect();
        BKP_DR1 = 1;
        pwr_enable_backup_domain_write_protect();


		/* Boot the application if it's valid. */
		if ((*(volatile uint32_t *)APP_ADDRESS & 0x2FFE0000) == 0x20000000) {
			/* Set vector table base address. */
			SCB_VTOR = APP_ADDRESS & 0xFFFF;
			/* Initialise master stack pointer. */
			asm volatile("msr msp, %0"::"g"
				     (*(volatile uint32_t *)APP_ADDRESS));
			/* Jump to application. */
			(*(void (**)())(APP_ADDRESS + 4))();
		}
	}

	rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_GPIOC);

	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13);
	usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config, usb_strings,4, usbd_control_buffer, sizeof(usbd_control_buffer));
	usbd_register_set_config_callback(usbd_dev, usbdfu_set_config);
	gpio_clear(GPIOC, GPIO13);

	while (1){

		usbd_poll(usbd_dev);
    }
}

uint8_t usbdfu_getstatus(usbd_device *usbd_dev, uint32_t *bwPollTimeout)
{
	(void)usbd_dev;

	switch (usbdfu_state) {
        case STATE_DFU_DNLOAD_SYNC:

            usbdfu_state = STATE_DFU_DNBUSY;
            *bwPollTimeout = 100;
            return DFU_STATUS_OK;
        case STATE_DFU_MANIFEST_SYNC:

            /* Device will reset when read is complete. */
            usbdfu_state = STATE_DFU_MANIFEST;
            return DFU_STATUS_OK;
        default:
            return DFU_STATUS_OK;
	}
}

void usbdfu_getstatus_complete(usbd_device *usbd_dev, struct usb_setup_data *req)
{
	int i;
	(void)req;
	(void)usbd_dev;

	switch (usbdfu_state) {

        case STATE_DFU_DNBUSY:
            flash_unlock();
            if (prog.blocknum == 0) {
                switch (prog.buf[0]) {
                case CMD_ERASE:
                    {
                        uint32_t *dat = (uint32_t *)(prog.buf + 1);
                        flash_erase_page(*dat);
                    }
                case CMD_SETADDR:
                    {
                        uint32_t *dat = (uint32_t *)(prog.buf + 1);
                        prog.addr = *dat;
                    }
                }
            } else {
                uint32_t baseaddr = prog.addr + ((prog.blocknum - 2) *
                           dfu_function.wTransferSize);
                for (i = 0; i < prog.len; i += 2) {
                    uint16_t *dat = (uint16_t *)(prog.buf + i);
                    flash_program_half_word(baseaddr + i,
                            *dat);
                }
            }
            flash_lock();

            /* Jump straight to dfuDNLOAD-IDLE, skipping dfuDNLOAD-SYNC. */
            usbdfu_state = STATE_DFU_DNLOAD_IDLE;
            return;
        case STATE_DFU_MANIFEST:

            //Ready to go to the application...

            //Reset the flag backup domain flag
            pwr_disable_backup_domain_write_protect();
            BKP_DR1 = 1;
            pwr_enable_backup_domain_write_protect();

            gpio_set(GPIOC,GPIO13);

            //Disable USB
            rcc_periph_clock_enable(RCC_GPIOA);

            //Disable the USB peripheral
            *((uint32_t*)( 0x40005C40)) |= 0x03;

            for(volatile int q = 0; q < 170000;q++);

            //Pull both lines low to tell the host the device is disonnected
            gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);
            gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
            gpio_clear(GPIOA,GPIO11);
            gpio_clear(GPIOA,GPIO12);

            for(volatile int q = 0; q < 170000;q++);

            //Disable the used clocks (power saving)
            rcc_periph_clock_disable(RCC_BKP);
            rcc_periph_clock_disable(RCC_PWR);
            rcc_periph_clock_disable(RCC_GPIOA);
            rcc_periph_clock_disable(RCC_GPIOC);

            scb_reset_system();
            return; /* Will never return. */
        default:
		return;
	}
}

int usbdfu_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf,
		uint16_t *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
	if ((req->bmRequestType & 0x7F) != 0x21)
		return 0; /* Only accept class request. */

	switch (req->bRequest) {
        case DFU_DNLOAD:
            if ((len == NULL) || (*len == 0)) {
                usbdfu_state = STATE_DFU_MANIFEST_SYNC;
                return 1;
            } else {
                /* Copy download data for use on GET_STATUS. */
                prog.blocknum = req->wValue;
                prog.len = *len;
                memcpy(prog.buf, *buf, *len);
                usbdfu_state = STATE_DFU_DNLOAD_SYNC;
                return 1;
            }
        case DFU_CLRSTATUS:
            /* Clear error and return to dfuIDLE. */
            if (usbdfu_state == STATE_DFU_ERROR)
                usbdfu_state = STATE_DFU_IDLE;
            return 1;
        case DFU_ABORT:
            /* Abort returns to dfuIDLE state. */
            usbdfu_state = STATE_DFU_IDLE;
            return 1;
        case DFU_UPLOAD:
            /* Upload not supported for now. */
            return 0;
        case DFU_GETSTATUS: {
            uint32_t bwPollTimeout = 0; /* 24-bit integer in DFU class spec */
            (*buf)[0] = usbdfu_getstatus(usbd_dev, &bwPollTimeout);
            (*buf)[1] = bwPollTimeout & 0xFF;
            (*buf)[2] = (bwPollTimeout >> 8) & 0xFF;
            (*buf)[3] = (bwPollTimeout >> 16) & 0xFF;
            (*buf)[4] = usbdfu_state;
            (*buf)[5] = 0; /* iString not used here */
            *len = 6;
            *complete = usbdfu_getstatus_complete;
            return 1;
            }
        case DFU_GETSTATE:
            /* Return state with no state transision. */
            *buf[0] = usbdfu_state;
            *len = 1;
            return 1;
        }

	return 0;
}

void usbdfu_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)wValue;

	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
				USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				(usbd_control_callback)usbdfu_control_request
                );
}
