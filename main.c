#include <xc.h>
#include <plib.h>

#include "usb_config.h"
#include "usb/usb.h"
#include "usb/usb_ch9.h"
#include "usb/usb_hid.h"


#pragma config DEBUG = OFF, ICESEL = ICS_PGx1, PWP = OFF, BWP = OFF, CP = OFF
#pragma config FNOSC = PRIPLL, FSOSCEN = OFF, IESO = OFF, POSCMOD = HS, OSCIOFNC = OFF
#pragma config FPBDIV = DIV_1, FCKSM = CSDCMD, WDTPS = PS1, FWDTEN = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_15, UPLLIDIV = DIV_2, UPLLEN = ON, FPLLODIV = DIV_1

#define KEY_NONE    0x00
#define KEY_ENTER   0x28

void DelayMs(int ms)
{
    { int i; for (i = 0; i < ms; i++)
    {
        WriteTimer1(0);
        while (ReadTimer1() < 10000);
    } }
}

int GetButtonState(void)
{
    return (PORTG & (1 << 6)) == 0;
}

void SendKey(char keyCode)
{
    if (usb_is_configured() && !usb_in_endpoint_halted(1) && !usb_in_endpoint_busy(1))
    {
        unsigned char *buf = usb_get_in_buffer(1);
        buf[0] = 0x00;
        buf[1] = keyCode;

        usb_send_in_buffer(1, 2);
    }
}

void Initialize(void)
{
    // Set Timer1 so delays work
    OpenTimer1(T1_ON | T1_IDLE_CON | T1_SOURCE_INT | T1_PS_1_8 | T1_GATE_OFF | T1_SYNC_EXT_OFF, 0xFFFF);

    // Multi-vector interrupts
    INTCONbits.MVEC = 1;
    // Interrupt priority, must set to != 0.
    IPC11bits.USBIP = 4;
    __asm volatile("ei");

    usb_init();

    // Make the button an input
    TRISG |= (1 << 6);
}

int main(void)
{
    Initialize();
    
    while (GetButtonState());
    while (1)
    {
        while (!GetButtonState());

        SendKey(KEY_ENTER);
        DelayMs(20);
        SendKey(KEY_NONE);
        DelayMs(100);

        while (GetButtonState());
        DelayMs(50);
    }

    return 0;
}




// Callbacks.

void SetConfigurationCallback(uint8_t configuration)
{

}

uint16_t GetDeviceStatusCallback()
{
	return 0x0000;
}

void EndpointHaltCallback(uint8_t endpoint, bool halted)
{

}

int8_t SetInterfaceCallback(uint8_t interface, uint8_t alt_setting)
{
	return 0;
}

int8_t GetInterfaceCallback(uint8_t interface)
{
	return 0;
}

void OutTransactionCallback(uint8_t endpoint)
{

}

void InTransactionCompleteCallback(uint8_t endpoint)
{

}

int8_t UnknownSetupRequestCallback(const struct setup_packet *setup)
{
	/* To use the HID device class, have a handler for unknown setup
	 * requests and call process_hid_setup_request() (as shown here),
	 * which will check if the setup request is HID-related, and will
	 * call the HID application callbacks defined in usb_hid.h. For
	 * composite devices containing other device classes, make sure
	 * MULTI_CLASS_DEVICE is defined in usb_config.h and call all
	 * appropriate device class setup request functions here.
	 */
	return process_hid_setup_request(setup);
}

int16_t UnknownGetDescriptorCallback(const struct setup_packet *pkt, const void **descriptor)
{
	return -1;
}

void StartOfFrameCallback(void)
{

}

void USBResetCallback(void)
{

}

// HID Callbacks.

static uint8_t report_buf[3];

static void GetReport(bool transfer_ok, void *context)
{
}

//int16_t app_get_report_callback
int16_t GetReportCallback(uint8_t interface, uint8_t report_type, uint8_t report_id, const void **report,
                                usb_ep0_data_stage_callback *callback, void **context)
{
	*report = report_buf;
	*callback = GetReport;
	*context = NULL;
	return sizeof(report_buf);
}

int8_t SetReportCallback(uint8_t interface, uint8_t report_type, uint8_t report_id)
{
	return -1;
}

uint8_t GetIdleCallback(uint8_t interface, uint8_t report_id)
{
	return 0;
}

int8_t SetIdleCallback(uint8_t interface, uint8_t report_id, uint8_t idle_rate)
{
	return -1;
}

int8_t GetProtocolCallback(uint8_t interface)
{
	return 1;
}

int8_t SetProtocolCallback(uint8_t interface, uint8_t report_id)
{
	return -1;
}
