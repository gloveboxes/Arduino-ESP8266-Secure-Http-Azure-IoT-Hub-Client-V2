#include "Bmp280.h"

void Bmp280::initialise()
{
  bmp280.begin();
  delay(100);
  initialised = true;
}

void Bmp280::measure()
{
  const int numberOfSamples = 2;

  initialise();
  temperature = pressure = humidity = 0;

  for (int c = 0; c < numberOfSamples; c++)
  {
    temperature += bmp280.readTemperature();
    pressure += (int)((int)(bmp280.readPressure() + 0.5) / 100);
    delay(100);
  }

  temperature /= numberOfSamples;
  pressure /= numberOfSamples;

  temperature = round(temperature * 10) / 10.0;
  pressure = round(pressure);
}
