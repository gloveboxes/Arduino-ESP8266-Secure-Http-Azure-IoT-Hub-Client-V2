#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson - installed via library manager
#include <TimeLib.h>     // http://playground.arduino.cc/code/time - installed via library manager
#include "DigitalPin.h"

class Sensor
{
public:
  Sensor()
  {
//    randomSeed(analogRead(A0));
  };

  void measure();
  char *toJSON();
  void setMsgId(unsigned int msgId)
  {
    this->msgId = msgId;
  }

  unsigned int getMsgId()
  {
    return msgId;
  }

  float temperature;
  float humidity;
  int pressure;
  int light;
  unsigned int msgId;
  const char *geo;
  const char *deviceId;
  int notSent;

protected:
  bool initialised;

private:
  char buffer[256];
  char isoTime[20];
  char *getISODateTime();
};

#endif
