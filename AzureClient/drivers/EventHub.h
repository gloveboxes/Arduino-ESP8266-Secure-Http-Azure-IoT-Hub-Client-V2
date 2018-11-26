//#ifndef Eventhub_h
//#define Eventhub_h
//
//#include "sha256.h"
//#include <Base64.h>
//#include "IotHub.h"
//
//class EventHub : public IotHub
//{
//
//  public:
//    EventHub(char *host, char *deviceId, char *eventHubPolicyName, char *key, char *eventHubName, const char *certificateFingerprint = "", time_t sasExpiryPeriodInSeconds = 15 * 60)
//        : IotHub(certificateFingerprint, sasExpiryPeriodInSeconds)
//    {
//        this->eventHubPolicyName = eventHubPolicyName;
//        this->eventHubName = eventHubName;
//
//        // device.host = (char *)malloc(strlen(host) + 1);
//        // strcpy(device.host, host);
//
//        device.host = "enviromon-eh.servicebus.windows.net/devices/messages";
//
//        device.id = (char *)malloc(strlen(deviceId) + 1);
//        strcpy(device.id, deviceId);
//
//        device.key = (char *)malloc(strlen(key) + 1);
//        strcpy(device.key, key);
//
//
//
//        snprintf(buff, sizeof(buff), EVENT_HUB_END_POINT, host, eventHubName);        
//        urlEncode(uri, buff);
//
//        snprintf(buff, sizeof(buff), ENDPOINT, host, eventHubName); 
//        
//        strcpy(endPoint, buff);
//
//        // sasUrl = String(buff);
//
//        //    initialiseHub();
//    }
//    // these two methods overide the virtual methods in the base class
//
//    //  void initialiseHub();
//
//  protected:
//  private:
//    char uri[200];
//    // String sasUrl;
//    bool generateSasToken();
//    void createSasToken(char *key);
//    char *eventHubPolicyName;
//
//    char *eventHubName;
//    const char *EVENT_HUB_END_POINT = "https://%s/%s";
//
//    const char * ENDPOINT = "/%s/%s/messages";
//};
//
//#endif
