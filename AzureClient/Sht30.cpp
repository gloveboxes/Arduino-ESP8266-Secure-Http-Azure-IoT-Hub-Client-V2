#include "Sht30.h"

void Sht30::initialise()
{
  if (initialised)
  {
    return;
  }

  if (!sht31.begin(0x45))
  { // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
  }

  delay(100);
  initialised = true;
}

void Sht30::measure()
{
  initialise();
  temperature = pressure = humidity = 0;

  temperature = sht31.readTemperature();
  humidity = sht31.readHumidity();

  temperature = round(temperature * 10) / 10.0;
  humidity = round(humidity * 10) / 10.0;
}
