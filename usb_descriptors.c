#include "usb_config.h"
#include "usb/usb.h"
#include "usb/usb_ch9.h"
#include "usb/usb_hid.h"

#define ROMPTR

struct configuration_1_packet
{
    struct configuration_descriptor  config;
    struct interface_descriptor      interface;
    struct hid_descriptor            hid;
    struct endpoint_descriptor       ep;
    struct endpoint_descriptor       ep1_out;
};


const ROMPTR struct device_descriptor DeviceDescriptor =
{
    sizeof(struct device_descriptor), // bLength
    DESC_DEVICE, // bDescriptorType
    0x0200, // 0x0200 = USB 2.0, 0x0110 = USB 1.1
    0x00, // Device class
    0x00, // Device Subclass
    0x00, // Protocol.
    EP_0_LEN, // bMaxPacketSize0
    0xA0A0, // Vendor
    0x0004, // Product
    0x0001, // device release (1.0)
    1, // Manufacturer
    2, // Product
    0, // Serial
    NUMBER_OF_CONFIGURATIONS // NumConfigurations
};

static const ROMPTR uint8_t KeyboardReportDescriptor[] =
{
    0x05, 0x01,         // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,         // USAGE (Keyboard)
    0xa1, 0x01,         // COLLECTION (Application)
    0x05, 0x07,         //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,         //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,         //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,         //   REPORT_SIZE (1)
    0x95, 0x08,         //   REPORT_COUNT (8)
    0x81, 0x02,         //   INPUT (Data,Var,Abs)
    0x95, 0x01,         //   REPORT_COUNT (1)
    0x75, 0x08,         //   REPORT_SIZE (8)
    0x25, 0x65,         //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,         //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,         //   INPUT (Data,Ary,Abs)
    0xc0                // END_COLLECTION
};

static const ROMPTR struct configuration_1_packet configuration_1 =
{
    {
        // Members from struct configuration_descriptor
        sizeof(struct configuration_descriptor),
        DESC_CONFIGURATION,
        sizeof(configuration_1), // wTotalLength (length of the whole packet)
        1, // bNumInterfaces
        1, // bConfigurationValue
        2, // iConfiguration (index of string descriptor)
        0b10000000,
        500/2,   // 100/2 indicates 100mA
    },

    {
        // Members from struct interface_descriptor
        sizeof(struct interface_descriptor), // bLength;
        DESC_INTERFACE,
        0x0, // InterfaceNumber
        0x0, // AlternateSetting
        0x2, // bNumEndpoints (num besides endpoint 0)
        HID_INTERFACE_CLASS, // bInterfaceClass 3=HID, 0xFF=VendorDefined
        0x00, // bInterfaceSubclass (0=NoBootInterface for HID)
        0x00, // bInterfaceProtocol
        0x02, // iInterface (index of string describing interface)
    },

    {
        // Members from struct hid_descriptor
        sizeof(struct hid_descriptor),
        DESC_HID,
        0x0101, // bcdHID
        0x0, // bCountryCode
        1,   // bNumDescriptors
        DESC_REPORT, // bDescriptorType2
        sizeof(KeyboardReportDescriptor), // wDescriptorLength
    },

    {
    // Members of the Endpoint Descriptor (EP1 IN)
        sizeof(struct endpoint_descriptor),
        DESC_ENDPOINT,
        0x01 | 0x80, // endpoint #1 0x80=IN
        EP_INTERRUPT, // bmAttributes
        EP_1_IN_LEN, // wMaxPacketSize
        1, // bInterval in ms.
    },

    {
        // Members of the Endpoint Descriptor (EP1 OUT)
        sizeof(struct endpoint_descriptor),
        DESC_ENDPOINT,
        0x01 /*| 0x00*/, // endpoint #1 0x00=OUT
        EP_INTERRUPT, // bmAttributes
        EP_1_OUT_LEN, // wMaxPacketSize
        1, // bInterval in ms.
    },
};

static const ROMPTR struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t lang; } str00 =
{
    sizeof(str00),
    DESC_STRING,
    0x0409 // US English
};

static const ROMPTR struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[23]; } vendor_string =
{
    sizeof(vendor_string),
    DESC_STRING,
    { 'S','i','g','n','a','l',' ','1','1',' ','S','o','f','t','w','a','r','e',' ','L','L','C','.' }
};

static const ROMPTR struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[14]; } product_string =
{
    sizeof(product_string),
    DESC_STRING,
    { 'U','S','B',' ','M','o','u','s','e',' ','D','e','m','o' }
};

static const ROMPTR struct {uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[11]; } interface_string =
{
    sizeof(interface_string),
    DESC_STRING,
    { 'I','n','t','e','r','f','a','c','e',' ','1' }
};

/* Get String function
 *
 * This function is called by the USB stack to get a pointer to a string
 * descriptor.  If using strings, USB_STRING_DESCRIPTOR_FUNC must be defined
 * to the name of this function in usb_config.h.  See
 * USB_STRING_DESCRIPTOR_FUNC in usb.h for information about this function.
 * This is a function, and not simply a list or map, because it is useful,
 * and advisable, to have a serial number string which may be read from
 * EEPROM or somewhere that's not part of static program memory.
 */
int16_t USBApplicationGetString(uint8_t stringNumber, const void **ptr)
{
    switch (stringNumber)
    {
        case 0:
            *ptr = &str00;
            return sizeof(str00);
        case 1:
            *ptr = &vendor_string;
            return sizeof(vendor_string);
        case 2:
            *ptr = &product_string;
            return sizeof(product_string);
        default:
            return -1;
    }
}

const struct configuration_descriptor *USBApplicationConfigDescs[] =
{
	(struct configuration_descriptor*) &configuration_1,
};
STATIC_SIZE_CHECK_EQUAL(USB_ARRAYLEN(USB_CONFIG_DESCRIPTOR_MAP), NUMBER_OF_CONFIGURATIONS);
STATIC_SIZE_CHECK_EQUAL(sizeof(USB_DEVICE_DESCRIPTOR), 18);

/* HID Descriptor Function */
int16_t USBApplicationGetHIDDescriptor(uint8_t interface, const void **ptr)
{
	/* Only one interface in this demo. The two-step assignment avoids an
	 * incorrect error in XC8 on PIC16. */
	const void *p = &configuration_1.hid;
	*ptr = p;
	return sizeof(configuration_1.hid);
}

/** HID Report Descriptor Function */
int16_t USBApplicationGetHIDReportDescriptor(uint8_t interface, const void **ptr)
{
	*ptr = KeyboardReportDescriptor;
	return sizeof(KeyboardReportDescriptor);
}
