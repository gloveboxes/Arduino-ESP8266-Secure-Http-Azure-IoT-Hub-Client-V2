#include "Bme280.h"

void Bme280::initialise(int powerPin)
{

  if (initialised && powerPin == -1)
  {
    return;
  }

  if (powerPin != -1)
  {
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, HIGH);
    delay(50); // wait for voltage to stabalise
  }

  this->powerPin = powerPin;

  bme280.begin(0x76);
  delay(100);
  initialised = true;
}

void Bme280::measure()
{
  const int numberOfSamples = 1;
  initialise();
  temperature = pressure = humidity = 0;

  for (int c = 0; c < numberOfSamples; c++)
  {
    temperature += bme280.readTemperature();
    pressure += (int)((int)(bme280.readPressure() + 0.5) / 100);
    humidity += bme280.readHumidity();
    if (numberOfSamples > 1)
    {
      delay(200);
    }
  }

  temperature /= numberOfSamples;
  pressure /= numberOfSamples;
  humidity /= numberOfSamples;

  if (powerPin != -1)
  {
    digitalWrite(powerPin, LOW);
  }

  Serial.println(temperature);
}
