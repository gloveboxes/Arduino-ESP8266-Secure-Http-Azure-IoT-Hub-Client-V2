#include "Sensor.h"

void Sensor::measure()
{
  temperature = 20 + random(-1, 1);
  humidity = 50 + random(-20, 40);
  pressure = 1000 + random(-100, 100);
}

char *Sensor::toJSON()
{
  /*  https://github.com/bblanchon/ArduinoJson/wiki/Memory-model
    Have allowed for a few extra json fields that actually being used at the moment
*/

  StaticJsonBuffer<JSON_OBJECT_SIZE(16)> jsonBuffer;

  JsonObject &root = jsonBuffer.createObject();
  root["DeviceId"] = deviceId;
  //  root["Utc"] = getISODateTime();
  root["Celsius"] = temperature;
  root["Humidity"] = humidity;
  root["hPa"] = pressure;
  root["Light"] = light;
  root["Geo"] = geo;
  root["Schema"] = 1;

  //instrumentation
  //  root["WiFi"] = telemetry->WiFiConnectAttempts;
#ifdef ARDUINO_ARCH_ESP8266
  root["Mem"] = ESP.getFreeHeap();
#endif

  root["Id"] = ++msgId;
  root["NotSent"] = notSent;

  root.printTo(buffer, sizeof(buffer));

  return buffer;
}

char *Sensor::getISODateTime()
{
  snprintf(isoTime, sizeof(isoTime), "%4d-%02d-%02dT%02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
  return isoTime;
}
