/*
  README

  PROJECT OWNER: Dave Glover | dglover@microsoft.com | @dglover

  COPYRIGHT: Free to use, reuse and modify, no libability accepted

  UPDATED: November 2018
  VERSION: 2.0

  PLATFORM: ESP8266
  FIRMWARE: Arduino core for ESP8266 WiFi chip V2.3.0 (https://github.com/esp8266/Arduino). Note, unstable on V2.4.x due to WiFi stack changes

  SUMMARY:
  This project was implemented and tested on the NodeMCU V2 (also known as V1.0), WeMos D1 Mini and Sparkfun ESP8266 Dev development boards on the ESP8266 platform
  flashed with Arduino core for ESP8266 WiFi chip V2.3.0 firmware.

  This solution streams sample sensor data securely over HTTPS / REST directly to Azure IoT Hub or Azure Event Hub.


  DEVELOPMENT BOARDS:
  NodeMCU: http://nodemcu.com/index_en.html#fr_54747661d775ef1a3600009e (This project will reflash the NodeMCU in to the Arduino platform. IMHO LUA not stable)
  WeMod D1 Mini: http://www.wemos.cc/wiki/doku.php?id=en:d1_mini
  Sparkfun 8266 Thing Dev Board: https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/introduction
  Other ESP8266 Development Boards - Adafruit, and others

  Sparkfun ESP8266 Dev Board Notes:

  With my limited experience with the Sparkfun ESP8266 Dev board I needed to use the Generic ESP8266 Arduino IDE Board Manager.
  See notes at https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/setting-up-arduino
  Change to Generic ESP8266 Module, and in the sub menus, make sure the following sub-menu's are also set:
	- Flash Mode: DIO
	- Flash Frequency: 80MHz
	- Upload Using: Serial
	- CPU Frequency: 80MHz
	- Flash Size: 512K (no SPIFFS)
	- Reset Method: nodemcu


  REQUIRED LIBRARIES: Install Arduino Libraries from Sketch -> Include Library -> Manage Libraries
  - ArduinoJson
  - NTPClient
  - Time
  - Adafruit Unified Sensor
  - Adafruit BME280
  - Adafruit SHT31


  CLOUD CONFIGURATION:

  The Azure IoT Hub device id, key and connection string can be obtained by right mouse clicking on a device in the Device Explorer.
  
  

  DRIVERS:
  - NodeMCU - On Windows, Mac and Linux you will need to install the latest CP210x USB to UART Bridge VCP Drivers. (https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
  - WeMos - On Windows and Mac install the latest Ch340G drivers. No drivers required for Linux. (http://www.wemos.cc/wiki/doku.php?id=en:ch340g)
  - Sparkfun ESP8266 Thing Dev - https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/hardware-setup


  ESP8266 ARDUINO IDE SUPPORT:

  1. Install the latest Arduino 1.8.7 as at Nov 2018 from the Arduino website.
  2. Start Arduino and open Preferences window.
  3. Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
  4. RESTART Arduino IDE
  5. Open Boards Manager from Tools > Board -> Boards Manager. Search for ESP8266 and install.
  6. Select NodeMUC or WeMos D1 Mini Board: Tools -> Board  -> NodeMCU 1.0 (ESP-12E module) or WeMos D1 Mini
  7. Set Port and Upload Speed: Tools. Note, you may need to try different port speeds to successfully flash the device.
	 - Faster is better as each time you upload the code to your device you are re-flashing the complete ROM not just your code, but faster can be less reliable.


  OPTIONAL COMPONENTS:

  Circuit Layout at https://github.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client

  - BMP180 Barometric Pressure Sensor
  - Adafruit Mini 8x8 LED Matrix w/I2C Backpack
  - Light Dependent Resistor
  - 10k resistor

  - WeMos Di Mini DHT(11) Shield: http://www.wemos.cc/wiki/doku.php?id=en:dht

*/

#include "Config.h"

#include <WiFiUdp.h>
#include <NTPClient.h>
#include "Device.h"


#include "Sensor.h"
#include "DigitalPin.h"
#include "Sht30.h"

//#include "Bme280.h"
//#include "Bmp280.h"
//#include "Bmp180.h"
//#include "DhtSensor.h"

Device device(WIFI_SSID, WIFI_PWD, PUBLISH_RATE_IN_SECONDS, DEEP_SLEEP_IN_SECONDS);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER);

#ifdef ARDUINO_ARCH_ESP8266
DigitalPin led(LED_BUILTIN, false, true); // initial state is off (false), invert true = high turns led off
#endif

#ifdef ARDUINO_ARCH_SAMD
DigitalPin led(LED_BUILTIN, false, false); // initial state is off (false)
#endif

// uncomment required sensor
Sensor sensor; // Fake sample environmental data useful for testing///
//Bme280 sensor;
//Sht30 sensor;/


// Structure which will be stored in RTC memory.
// First field is CRC32, which is calculated based on the
// rest of structure contents.
// Any fields can go after CRC32.
// We use byte array as an example.
struct
{
  uint32_t crc32;
  unsigned int msgId;
  time_t lastSentEpoch;
  int lastTemperature;
  int lastPressure;
  int lastHumidity;
  int notSent;
} rtcData;

void setup()
{
  Serial.begin(115200);

  sensor.deviceId = hub.getDeviceId();
  sensor.geo = DEVICE_LOCATION;

#ifdef ARDUINO_ARCH_ESP8266
  if (device.deepSleepSeconds > 0)
  {
    lowPowerPublishESP8266();
  }
#endif

#if defined ARDUINO_SAMD_FEATHER_M0
  //  Required for feather m0 wifi
  WiFi.setPins(8, 7, 4, 2);
#endif

  device.connectWifi();

  timeClient.begin(); // must be called before setSyncProvider

  setSyncProvider(getCurrentTime); // This initiates an immediate call to getCurrentTime
  setSyncInterval(60 * 60 * 12);   // update time every twelve hours

  Serial.println(now());

  delay(100);
  Serial.println("");
}

void loop()
{
  sensor.measure();

  //  sensor.light = ldr.measure();
  //  display.updateDisplay();

  device.connectWifi();

  led.on();
  int resultCode = hub.publish(sensor.toJSON()); // resultCode 204 IoTHub Sucess, 201 EventHub Success
  led.off();

  Serial.print(sensor.deviceId);
  Serial.println(" :" + String(resultCode));

  delay(device.publishRateInSeconds * 1000); // limit publishing rate
}

#ifdef ARDUINO_ARCH_ESP8266
void lowPowerPublishESP8266()
{
  bool rtcDataFound = false;
  bool publish = false;

  // Retrieve msgId from rtc after deepsleep
  if (ESP.rtcUserMemoryRead(0, (uint32_t *)&rtcData, sizeof(rtcData)))
  {
    uint32_t crcOfData = device.calculateCRC32(((uint8_t *)&rtcData) + 4, sizeof(rtcData) - 4);
    if (crcOfData == rtcData.crc32)
    {
      sensor.msgId = rtcData.msgId;
      sensor.notSent = rtcData.notSent;
      rtcDataFound = true;
    }
  }

  if (!rtcDataFound)
  {
    rtcData.msgId = rtcData.lastTemperature = rtcData.lastPressure = rtcData.lastHumidity = rtcData.notSent = 0;
    rtcData.lastSentEpoch = 0;
  }

  sensor.measure();
  //  sensor.light = ldr.measure();

  int currentTemperature = sensor.temperature * 10;
  int currentPressure = sensor.pressure * 10;
  int currentHumidity = sensor.humidity * 10;

  if (rtcDataFound)
  {
    // if difference great than .3 degree
    if (abs(rtcData.lastTemperature - currentTemperature) > 3)
    {
      publish = true;
    }
  }
  else
  {
    publish = true; // always publish if rtcDataFound false, ie no valid rtc data
  }

  if (publish)
  {
    device.connectWifi();

    timeClient.begin();              // must be called before setSyncProvider
    setSyncProvider(getCurrentTime); // This initiates an immediate call to getCurrentTime

    rtcData.msgId = sensor.msgId;
    rtcData.lastSentEpoch = now();

    int resultCode = hub.publish(sensor.toJSON()); // resultCode 204 IoTHub Success, 201 EventHub Success

    // only update rtc data if sucessefully published data
    if (resultCode == 201 || resultCode == 204)
    {

      rtcData.lastTemperature = currentTemperature;
      rtcData.lastHumidity = currentHumidity;
      rtcData.lastPressure = currentPressure;
      rtcData.msgId = sensor.getMsgId();

      rtcData.crc32 = device.calculateCRC32(((uint8_t *)&rtcData) + 4, sizeof(rtcData) - 4);
      ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, sizeof(rtcData));
    }
  }
  else
  {
    rtcData.notSent++;

    rtcData.crc32 = device.calculateCRC32(((uint8_t *)&rtcData) + 4, sizeof(rtcData) - 4);
    ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, sizeof(rtcData));
  }

  WiFi.mode(WIFI_OFF);
  ESP.deepSleep(1000000 * device.deepSleepSeconds, WAKE_RF_DEFAULT); // GPIO16 needs to be tied to RST to wake from deepSleep. Execute restarts from beginning of sketch
}
#endif

time_t getCurrentTime()
{
  while (!timeClient.forceUpdate())
  {
    delay(100);
  }
  return timeClient.getEpochTime();
}
