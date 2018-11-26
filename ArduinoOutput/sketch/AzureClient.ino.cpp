#include <Arduino.h>
#line 1 "/home/dave/Github/Arduino-ESP8266-Secure-Http-Azure-IoT-Hub-Client-V2/AzureClient/AzureClient.ino"
#line 1 "/home/dave/Github/Arduino-ESP8266-Secure-Http-Azure-IoT-Hub-Client-V2/AzureClient/AzureClient.ino"
/*
  README

  PROJECT OWNER: Dave Glover | dglover@microsoft.com | @dglover

  COPYRIGHT: Free to use, reuse and modify, no libability accepted

  UPDATED: April 2018
  VERSION: 2.0

  SUMMARY:
  This project was implemented and tested on the NodeMCU V2 (also known as V1.0), WeMos D1 Mini and Sparkfun ESP8266 Dev development boards on the ESP8266 platform
  flashed with Arduino core for ESP8266 WiFi chip V2.0 firmware.

  This solution streams sample sensor data securely over HTTPS / REST directly to Azure IoT Hub or Azure Event Hub.

  PLATFORM: ESP8266

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


  FIRMWARE: Arduino core for ESP8266 WiFi chip V2.0 | https://github.com/esp8266/Arduino

  ARDUINO IDE: 4.6.5 (as at Jan 2016)
  - ESP8266 V2 requires Arduino IDE 4.6.5 (high versions will result is scope compiler errors
  - http://esp8266.github.io/Arduino/versions/2.0.0/doc/installing.html


  REQUIRED LIBRARIES: Install Arduino Libraries from Sketch -> Include Library -> Manage Libraries
  - ArduinoJson
  - NTPClient
  - Time
  - rBase64
  - Adafruit BMP085 (Unified version)
  - Adafruit Unified Sensor
  - Adafruit BMP280
  - Adafruit BME280
  - Adafruit SHT31


  CLOUD CONFIGURATION:

  The Azure IoT Hub device id, key and connection string can be obtained by right mouse clicking on a device in the Device Explorer.
  
  

  DRIVERS:
  - NodeMCU - On Windows, Mac and Linux you will need to install the latest CP210x USB to UART Bridge VCP Drivers. (https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
  - WeMos - On Windows and Mac install the latest Ch340G drivers. No drivers required for Linux. (http://www.wemos.cc/wiki/doku.php?id=en:ch340g)
  - Sparkfun ESP8266 Thing Dev - https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/hardware-setup


  ESP8266 ARDUINO IDE SUPPORT:

  1. Install the latest Arduino 1.8.5 as at Nov 2017 from the Arduino website.
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

#define DEVICE "wemos-mlb"
#define DEVICE_LOCATION "mlb-study"

#define WIFI_SSID "NCW"
#define WIFI_PWD "malolos5459"

#define DEEP_SLEEP_IN_SECONDS 0
#define PUBLISH_RATE_IN_SECONDS 2

#define IOTHUB_CONNECTION_STRING "HostName=glovebox-iothub.azure-devices.net;DeviceId=wemos-fake;SharedAccessKey=n9xt+hMC1xdssxjBk88UWFr9Aiqgh6XOSi8uJDWPHns="
// #define EVENTHUB_CONNECTION_STRING DUMMY

// Azure IoT Hub Settings
#ifdef IOTHUB_CONNECTION_STRING

//#define IOTHUB_CERTIFICATE_FINGERPRINT "9C:AC:A9:4C:32:F9:53:0A:85:CA:AA:4D:CE:A2:D9:A2:83:9A:B8:AB"
#define IOTHUB_CERTIFICATE_FINGERPRINT ""

#include "IotHub.h"
IotHub hub(IOTHUB_CONNECTION_STRING, IOTHUB_CERTIFICATE_FINGERPRINT);

#endif

// Azure Event Hub Settings
#ifdef EVENTHUB_CONNECTION_STRING

#define EVENTHUB_HOST "enviromon-eh.servicebus.windows.net"
#define EVENTHUB_POLICY "Sender"
#define EVENTHUB_KEY "LNK8aM4Y15I+AWsg7V9PowWpa81DyJLa+DkWddaI4lk="
//#define EVENTHUB_CERT_FINGERPRINT "BB:F4:6C:1B:20:8B:DF:1D:0A:39:96:E0:5E:02:5E:D2:9E:60:C1:56"
#define EVENTHUB_CERT_FINGERPRINT ""
#define EVENTHUB_HUBNAME "devices"

#include "EventHub.h"
EventHub hub(EVENTHUB_HOST, DEVICE, EVENTHUB_POLICY, EVENTHUB_KEY, EVENTHUB_HUBNAME, EVENTHUB_CERT_FINGERPRINT);

#endif

#define NTP_SERVER "au.pool.ntp.org"

/* 
 http://hassansin.github.io/certificate-pinning-in-nodejs
 for information on generating fingerprint
 From Ubuntu subsystem on Windows 10
 echo -n | openssl s_client -connect IoTCampAU.azure-devices.net:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > cert.pem
 openssl x509 -noout -in cert.pem -fingerprint
*/

#include <WiFiUdp.h>
#include <NTPClient.h>
#include "Device.h"
//#include "Oled.h"

#include "Sensor.h"
#include "DigitalPin.h"
//#include "Ldr.h"
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
//Bmp180 sensor;
//Bmp280 sensor;
//DhtSensor sensor(device, dht11);
//DhtSensor sensor(device, dht22);

//Ldr ldr;
//OLED display(&sensor);

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

  sensor.deviceId = DEVICE;
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

