
#define WIFI_SSID "[Your WiFi SSID]"
#define WIFI_PWD "[Your WiFi Password]"

#define NTP_SERVER "au.pool.ntp.org"

#define DEVICE_LOCATION "[Your device location - eg Garage]"

#define DEEP_SLEEP_IN_SECONDS 0
#define PUBLISH_RATE_IN_SECONDS 60

#define IOTHUB_CONNECTION_STRING "[Your IoT Hub/Central Connection String eg HostName=saas-iothub-897898-f33a-4002-a44a-7ca5961b00b6.azure-devices.net;DeviceId=wemos-garage;SharedAccessKey=uRsr78d56s87878dd89d675g88H8+nRoiVgc3+epqlMHHg="



#ifdef IOTHUB_CONNECTION_STRING

//#define IOTHUB_CERTIFICATE_FINGERPRINT "9C:AC:A9:4C:32:F9:53:0A:85:CA:AA:4D:CE:A2:D9:A2:83:9A:B8:AB"
#define IOTHUB_CERTIFICATE_FINGERPRINT ""

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



/* 
 http://hassansin.github.io/certificate-pinning-in-nodejs
 for information on generating fingerprint
 From Ubuntu subsystem on Windows 10
 echo -n | openssl s_client -connect IoTCampAU.azure-devices.net:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > cert.pem
 openssl x509 -noout -in cert.pem -fingerprint
*/
