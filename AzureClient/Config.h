
#define WIFI_SSID "[Your WiFi SSID]"
#define WIFI_PWD "[Your WiFi Password]"

#define NTP_SERVER "au.pool.ntp.org"

#define DEVICE_LOCATION "[Your device location - eg Garage]"

#define DEEP_SLEEP_IN_SECONDS 0
#define PUBLISH_RATE_IN_SECONDS 60

#define IOTHUB_CONNECTION_STRING "[Your Azure IoT Hub or Azure IoT Central Connection String]"

/* 
 http://hassansin.github.io/certificate-pinning-in-nodejs for information on generating fingerprint. From Ubuntu or Ubuntu subsystem on Windows 10
 
 echo -n | openssl s_client -connect [your azure iot hub].azure-devices.net:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > cert.pem
 openssl x509 -noout -in cert.pem -fingerprint 
*/

#define IOTHUB_CERTIFICATE_FINGERPRINT "95:B4:61:DF:90:D9:D7:1D:15:22:D8:DB:2E:F1:7D:BC:F4:BB:41:D2"
// #define IOTHUB_CERTIFICATE_FINGERPRINT ""

#ifdef IOTHUB_CONNECTION_STRING

#include "IotHub.h"
IotHub hub(IOTHUB_CONNECTION_STRING, IOTHUB_CERTIFICATE_FINGERPRINT);

#endif



/*


// Azure Event Hub Settings
#ifdef EVENTHUB_CONNECTION_STRING

#define EVENTHUB_HOST "enviromon-eh.servicebus.windows.net"
#define EVENTHUB_POLICY "Sender"
#define EVENTHUB_KEY "LNK8aM4Y15I+AWsg7V9PowWpa81DyJLa+DkWddaI4lk="
//#define EVENTHUB_CERT_FINGERPRINT "BB:F4:6C:1B:20:8B:DF:1D:0A:39:96:E0:5E:02:5E:D2:9E:60:C1:56"
#define EVENTHUB_CERT_FINGERPRINT ""
#define EVENTHUB_HUBNAME "devices"

#include "EventHub.h"
EventHub hub(EVENTHUB_HOST, DEVICE, EVENTHUB_POLICY, EVENTHUB_KEY, EVENTHUB_HUBNAME, EVENTHUB_CERT_FINGERPRINT);

#endif

*/
