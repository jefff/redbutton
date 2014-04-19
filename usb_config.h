#ifndef USB_CONFIG_H__
#define USB_CONFIG_H__

/* Number of endpoint numbers besides endpoint zero. It's worth noting that
   and endpoint NUMBER does not completely describe an endpoint, but the
   along with the DIRECTION does (eg: EP 1 IN).  The #define below turns on
   BOTH IN and OUT endpoints for endpoint numbers (besides zero) up to the
   value specified.  For example, setting NUM_ENDPOINT_NUMBERS to 2 will
   activate endpoints EP 1 IN, EP 1 OUT, EP 2 IN, EP 2 OUT.  */
#define NUM_ENDPOINT_NUMBERS 1

/* Only 8, 16, 32 and 64 are supported for endpoint zero length. */
#define EP_0_LEN 8

#define EP_1_OUT_LEN 8
#define EP_1_IN_LEN 8

#define NUMBER_OF_CONFIGURATIONS 1

/* Ping-pong buffering mode. Valid values are:
	PPB_NONE         - Do not ping-pong any endpoints
	PPB_EPO_OUT_ONLY - Ping-pong only endpoint 0 OUT
	PPB_ALL          - Ping-pong all endpoints
	PPB_EPN_ONLY     - Ping-pong all endpoints except 0
*/

#define PPB_MODE PPB_ALL


/* Comment the following line to use polling USB operation. When using polling,
   You are responsible for calling usb_service() periodically from your
   application. */
#define USB_USE_INTERRUPTS

/* Uncomment if you have a composite device which has multiple different types
 * of device classes. For example a device which has HID+CDC or
 * HID+VendorDefined, but not a device which has multiple of the same class
 * (such as HID+HID). Device class implementations have additional requirements
 * for multi-class devices. See the documentation for each device class for
 * details. */
//#define MULTI_CLASS_DEVICE


/* Objects from usb_descriptors.c */
#define USB_DEVICE_DESCRIPTOR DeviceDescriptor
#define USB_CONFIG_DESCRIPTOR_MAP USBApplicationConfigDescs
#define USB_STRING_DESCRIPTOR_FUNC USBApplicationGetString

/* Optional callbacks from usb.c. Leave them commented if you don't want to
   use them. For the prototypes and documentation for each one, see usb.h. */

#define SET_CONFIGURATION_CALLBACK SetConfigurationCallback
#define GET_DEVICE_STATUS_CALLBACK GetDeviceStatusCallback
#define ENDPOINT_HALT_CALLBACK     EndpointHaltCallback
#define SET_INTERFACE_CALLBACK     SetInterfaceCallback
#define GET_INTERFACE_CALLBACK     GetInterfaceCallback
#define OUT_TRANSACTION_CALLBACK   OutTransactionCallback
#define IN_TRANSACTION_COMPLETE_CALLBACK   InTransactionCompleteCallback
#define UNKNOWN_SETUP_REQUEST_CALLBACK UnknownSetupRequestCallback
#define UNKNOWN_GET_DESCRIPTOR_CALLBACK UnknownGetDescriptorCallback
#define START_OF_FRAME_CALLBACK    StartOfFrameCallback
#define USB_RESET_CALLBACK         USBResetCallback

/* HID Configuration functions. See usb_hid.h for documentation. */
#define USB_HID_DESCRIPTOR_FUNC USBApplicationGetHIDDescriptor
#define USB_HID_REPORT_DESCRIPTOR_FUNC USBApplicationGetHIDReportDescriptor
//#define USB_HID_PHYSICAL_DESCRIPTOR_FUNC usb_application_get_hid_physical_descriptor

/* HID Callbacks. See usb_hid.h for documentation. */
#define HID_GET_REPORT_CALLBACK GetReportCallback
#define HID_SET_REPORT_CALLBACK SetReportCallback
#define HID_GET_IDLE_CALLBACK GetIdleCallback
#define HID_SET_IDLE_CALLBACK SetIdleCallback
#define HID_GET_PROTOCOL_CALLBACK GetProtocolCallback
#define HID_SET_PROTOCOL_CALLBACK SetProtocolCallback

#endif /* USB_CONFIG_H__ */
