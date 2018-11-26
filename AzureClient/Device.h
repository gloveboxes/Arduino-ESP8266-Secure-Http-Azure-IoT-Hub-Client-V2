#ifndef Device_h
#define Device_h

//#include <arduino.h>
//#include <EEPROM.h>
//#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson - installed via library manager

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#endif

#ifdef ARDUINO_ARCH_SAMD
#include <WiFi101.h>
#endif

class Device
{
public:
  Device(const char *ssid, const char *pwd, int publishRateInSeconds = 60, int deepSleepSeconds = 0)
  {
#ifdef ARDUINO_ARCH_ESP8266
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA); // Ensure WiFi in Station/Client Mode
#endif
    wifiPairs = 1;
    _ssid = new const char *[wifiPairs];
    _pwd = new const char *[wifiPairs];
    _ssid[0] = ssid;
    _pwd[0] = pwd;
    this->publishRateInSeconds = (publishRateInSeconds < 0) ? 0 : publishRateInSeconds;
    this->deepSleepSeconds = (deepSleepSeconds < 0) ? 0 : deepSleepSeconds;
  }

  bool connectWifi();

  int deepSleepSeconds = 0;      // Number of seconds for the ESP8266 chip to deepsleep for.  GPIO16 needs to be tied to RST to wake from deepSleep http://esp8266.github.io/Arduino/versions/2.0.0/doc/libraries.html
  int publishRateInSeconds = 60; // defaults to once a minute
  unsigned long lastPublishTime = 0;
  uint32_t calculateCRC32(const uint8_t *data, size_t length);

private:
  //    const char *GetValue(const char *value);
  int WifiIndex = 0;
  int wifiPairs = 0;
  const char **_ssid;
  const char **_pwd;
  unsigned long LastWifiTime = 0;
};

#endif
