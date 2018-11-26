//#include "EventHub.h"
//
//// void EventHub::initialiseHub()
//// {
////     snprintf(endPoint, sizeof(endPoint), EVENT_HUB_END_POINT, eventHubName, eventHubPolicyName);
//
////     sasUrl = "https://" + (String)host + endPoint;
////     urlEncode(buff, (char *)sasUrl.c_str());
////     sasUrl = String(buff);
//// }
//
//void EventHub::createSasToken(char *key)
//{
//  // START: Create SAS
//  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
//
//  sas.expiryTime = now() + sas.expiryPeriodInSeconds;
//
//  String stringToSign = String(uri) + "\n" + sas.expiryTime;
//
//  Serial.println(stringToSign);
//
//  // START: Create signature
//  Sha256.initHmac((const uint8_t *)key, 44);
//
//  // char* buffPrt = buff;
//  // buffPrt += urlEncode(buff, "https://");
//  // snprintf(buffPrt, sizeof(buff), "%s/%s\n%d", this->device.host, this->eventHubName, sas.expiryTime);
//  // Serial.println(buff);
//  
//  Sha256.print(stringToSign);
//
//  // Sha256.print(stringToSign);
//
//  char *sign = (char *)Sha256.resultHmac();
//  int signLen = 32;
//  // END: Create signature//  // START: Get base64 of signature
//
//  int encodedSignLen = Base64.encodedLength(signLen);
//  memset(buff, 0, encodedSignLen + 1); // initialised extra byte to allow for null termination
//  Base64.encode(buff, sign, signLen);
//
//  char *sasPointer = sas.token;
//  sasPointer += snprintf(sasPointer, sizeof(sas.token), "SharedAccessSignature sr=%s&sig=", this->uri);
//  sasPointer += urlEncode(sasPointer, buff);
//  snprintf(sasPointer, sizeof(sas.token) - (sasPointer - sas.token), "&se=%d&skn=%s", sas.expiryTime, eventHubPolicyName);
//
//  Serial.println(sas.token);
//  snprintf(sas.token, sizeof(sas.token), "%s", "SharedAccessSignature sr=https%3A%2F%2Fenviromon-eh.servicebus.windows.net%2Fdevices&sig=Ndwg%2BXr7NDPnmVsaGFK8yYhUd64peM95ODPw0n0bL0g%3D&se=1542887878&skn=Sender");
//
//  Serial.println(sas.token);
//}
