#ifndef Bme280_h
#define Bme280_h

#include "Sensor.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class Bme280 : public Sensor
{
public:
  Bme280(){};
  void measure();
  void initialise(int powerPin = -1);

protected:
  int powerPin = -1;
  Adafruit_BME280 bme280; // I2C
};

#endif
