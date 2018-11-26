#ifndef IoTHub_hV2
#define IoTHub_hV2

#ifdef ARDUINO_ARCH_ESP8266
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#endif

#ifdef ARDUINO_ARCH_SAMD
#include <WiFi101.h>
#endif

#include "SasToken.h"

#define BUFSIZE 512
#define SEGMENT_LENGTH 150

class IotHub : public SasToken
{
public:
  //  IotHub(char *host, char *deviceId, char *key, const char *certificateFingerprint, time_t sasExpiryPeriodInSeconds = 15 * 60)
  IotHub(char *connectionString, const char *certificateFingerprint, time_t sasExpiryPeriodInSeconds = 15 * 60)
  {
    this->certificateFingerprint = certificateFingerprint;
    this->sas.expiryPeriodInSeconds = sasExpiryPeriodInSeconds;

    tokeniseConnectionString(connectionString);

    urlEncode(buff, (char *)TARGET_URL);
    snprintf(endPoint, sizeof(endPoint), "%s%s%s", TARGET_URL, device.id, IOT_HUB_END_POINT);
  }

  IotHub(const char *certificateFingerprint, time_t sasExpiryPeriodInSeconds = 15 * 60)
  {
    this->certificateFingerprint = certificateFingerprint;
    this->sas.expiryPeriodInSeconds = sasExpiryPeriodInSeconds;
  }

  int publishBegin(int dataLength);
  int publishData(char *data, int dataLength);
  int publishEnd();
  int publish(char *data);
  char *getDeviceId()
  {
    return this->device.id;
  }

protected:
  char buff[BUFSIZE];
  char endPoint[200];
  //  time_t currentEpochTime();
  //  char fullSas[200];

private:
  //https://forum.arduino.cc/index.php?topic=129535.0
  //ARDUINO_ARCH_SAM
  //ARDUINO_ARCH_SAMD
  //ARDUINO_ARCH_AVR
  //ARDUINO_ARCH_ESP8266

#ifdef ARDUINO_ARCH_ESP8266
  WiFiClientSecure *client = new WiFiClientSecure();
#endif

#ifdef ARDUINO_ARCH_SAMD
  WiFiSSLClient *client = new WiFiSSLClient;
#endif

  void tokeniseConnectionString(char *cs);
  char *getValue(char *token, char *key);
  const char *certificateFingerprint;
  //  void generateSas();
  //  virtual void initialiseHub();

  bool verifyServerFingerprint();

  const char *TARGET_URL = "/devices/";
  const char *IOT_HUB_END_POINT = "/messages/events?api-version=2018-06-30";
  const char *httpRequest = "POST %s HTTP/1.1\r\nHost: %s\r\nAuthorization: %s\r\nContent-Type: application/atom+xml;type=entry;charset=utf-8\r\nContent-Length: %d\r\n\r\n";
};

#endif
