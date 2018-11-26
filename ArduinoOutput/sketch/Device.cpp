#include "Device.h"

bool Device::connectWifi()
{
  bool newConnection = false;
  LastWifiTime = 0;
  const int WifiTimeoutMilliseconds = 30000; // 60 seconds
  int status;

#ifdef ARDUINO_ARCH_ESP8266
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA); // Ensure WiFi in Station/Client Mode
  }
#endif

#ifdef ARDUINO_ARCH_SAMD
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true)
      ;
  }
#endif

  while (WiFi.status() != WL_CONNECTED)
  {
    newConnection = true;

    if (millis() < LastWifiTime)
    {
      Serial.print(".");
      delay(250);
      continue;
    }

    if (WifiIndex >= wifiPairs)
    {
      WifiIndex = 0;
    }

    Serial.println("trying " + String(_ssid[WifiIndex]));

    WiFi.begin(_ssid[WifiIndex], _pwd[WifiIndex]);

    LastWifiTime = millis() + WifiTimeoutMilliseconds;

    WifiIndex++; //increment wifi indexready for the next ssid/pwd pair in case the current wifi pair dont connect
  }

  if (newConnection)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }

  return newConnection;
}

uint32_t Device::calculateCRC32(const uint8_t *data, size_t length)
{
  uint32_t crc = 0xffffffff;
  while (length--)
  {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1)
    {
      bool bit = crc & 0x80000000;
      if (c & i)
      {
        bit = !bit;
      }
      crc <<= 1;
      if (bit)
      {
        crc ^= 0x04c11db7;
      }
    }
  }
  return crc;
}
