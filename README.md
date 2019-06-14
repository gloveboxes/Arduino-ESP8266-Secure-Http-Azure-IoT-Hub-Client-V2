# Securely stream data from ESP8266 MCUs to Azure IoT Hub or Azure IoT Central

|Author|[Dave Glover](https://developer.microsoft.com/en-us/advocates/dave-glover?WT.mc_id=article-github-dglover), Microsoft Cloud Developer Advocate |
|----|---|
|Solution|This solution securely streams sensor data directly to Azure IoT Hub over HTTPS calling Azure REST APIs from an ESP8266 MCU.|
|Source Code| [ESP8266 Secure Azure IoT Hub Client V2](https://github.com/gloveboxes/Arduino-ESP8266-Secure-Http-Azure-IoT-Hub-Client-V2) |
|Documentation|[README](https://gloveboxes.github.io/Arduino-ESP8266-Secure-Http-Azure-IoT-Hub-Client-V2)|
|Platform|[ESP8266 (Firmware V2.3.0)](https://github.com/esp8266/Arduino), [Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/?WT.mc_id=article-github-dglover), [Azure IoT Hub](https://docs.microsoft.com/en-au/azure/iot-hub/?WT.mc_id=article-github-dglover), [Azure Stream Analytics](https://docs.microsoft.com/en-us/azure/stream-analytics/?WT.mc_id=article-github-dglover), [Power Bi](https://docs.microsoft.com/en-us/power-bi/?WT.mc_id=article-github-dglover)|
|Video Training|[What is Azure IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/overview-iot-central/?WT.mc_id=article-github-dglover), [Introduction to Azure IoT Hub](https://www.youtube.com/watch?v=smuZaZZXKsU)|
|Screencasts|[How to create the Azure IoT Central Application](https://youtu.be/D26rJmHyZcA), [How to create an Azure IoT Hub](https://youtu.be/lHpUu6wSX40)|
|Date|As at Nov 2018|
|Acknowledgment|Thanks to [Štěpán Bechynský](https://microsoft.hackster.io/en-US/stepanb) "[Proof of Concept – NodeMCU, Arduino and Azure Event Hub] (https://microsoft.hackster.io/en-US/stepanb/proof-of-concept-nodemcu-arduino-and-azure-event-hub-a33043)" project |

## Device Platform

The [ESP8266](https://en.wikipedia.org/wiki/ESP8266) is a great commodity priced [Arduino](https://github.com/esp8266/Arduino) 
compatible MCU with integrated WiFi

This project is implemented and tested on the following ESP8266 based development boards:

1. [NodeMCU V2 (also known as V1.0)](https://en.wikipedia.org/wiki/NodeMCU),
2. [WeMos D1 Mini](http://www.wemos.cc/) 
3. and [SparkFun ESP8266 Thing](https://www.sparkfun.com/products/13711)

## Firmware

[Arduino core for ESP8266 WiFi chip V2.3.0](https://github.com/esp8266/Arduino) firmware adds HTTPS ([TLS](http://axtls.sourceforge.net/) 1.0 and 1.1) support, making this a viable platform for secure IoT data streaming. See [Security Discussion](https://github.com/esp8266/Arduino/issues/43) for more information.

## IoT Hub Certificate Verification

This sample verifies your IoT Hub Server Certificate to mitigate against Man in the Middle Attacks.

The Server Certificate Fingerprint was generated as follows:-

1. From Bash on Ubuntu on Windows (10) or Linux
2. echo -n | openssl s_client -connect IoTCampAU.azure-devices.net:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > cert.pem
3. openssl x509 -noout -in cert.pem -fingerprint
4. [See Certificate Pinning article for more information](http://hassansin.github.io/certificate-pinning-in-nodejs)

## Azure IoT Hub

IoT Hub is designed to "Securely connect, monitor, and control millions of IoT assets" and is
designed for internet scale data ingestion.

[Stream Analytics](https://docs.microsoft.com/en-us/azure/stream-analytics/?WT.mc_id=article-github-dglover),
[Power Bi](https://docs.microsoft.com/en-us/power-bi/?WT.mc_id=article-github-dglover) and preconfigured IoT Hub solutions such as
[Remote monitoring](https://docs.microsoft.com/en-us/azure/iot-accelerators/iot-accelerators-remote-monitoring-sample-walkthrough/?WT.mc_id=article-github-dglover) provide ways to visualise and unlock the value of your data in Azure.

For more background information read this "[Stream Analytics & Power BI: A real-time analytics dashboard for streaming data](https://docs.microsoft.com/en-us/azure/stream-analytics/stream-analytics-power-bi-dashboard/?WT.mc_id=article-github-dglover)" article.

## Setup and Deployment Summary

1. Setup your Azure IoT Hub. There is a free 8000 message a day subscription to get started.
2. Register your device with Azure IoT Hub.
3. Update the main AzureClient.ino sketch
4. Deploy the solution to your ESP8266 based device.
5. View data with Device Explorer
6. Optionally: Visualise your data in real time with Azure Stream Analytics and Power BI.

## Setting Up Azure IoT Hub

Follow lab [Setting Up Azure IoT](http://thinglabs.io/workshop/cs/nightlight/setup-azure-iot-hub/) to provision an Azure IoT Hub and an IoT Hub device.

## Solution Configuration

Open the Config.h file in the solution and configure the following:

```c
#define WIFI_SSID "[Your WiFi SSID]"
#define WIFI_PWD "[Your WiFi Password]"

#define NTP_SERVER "au.pool.ntp.org"

#define DEVICE_LOCATION "[Your device location - eg Garage]"

#define DEEP_SLEEP_IN_SECONDS 0
#define PUBLISH_RATE_IN_SECONDS 60

#define IOTHUB_CONNECTION_STRING "[Your IoT Hub/Central Connection String eg HostName=saas-iothub-897898-f33a-4002-a44a-7ca5961b00b6.azure-devices.net;DeviceId=wemos-garage;SharedAccessKey=uRsr78d56s87878dd89d675g88H8+nRoiVgc3+epqlMHHg="
```

### Select Sensor

The sample includes the following sensors in the main project: Fake Sensor,
BME280, and the Sht30. The default sensor returns fake temperature, humidity, pressure and light telemetry. To switch to a real sensor comment out the Sensor sensor line and uncomment one of real sensors.

```c
// uncomment required sensor
Sensor sensor; // Fake sample environmental data useful for testing///
//Bme280 sensor;
//Sht30 sensor;/
```

There are additional drivers in the drivers directory: BMP180, BMP280, DHT11, DHT22, LDR, OLED.

## Viewing Data

From Device Explorer, head to the Data tab, select your device, enable consumer group then click Monitor.

![IoT Hub Data](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/IoTHubData.JPG)

## Visualising Data

### Azure Stream Analytics

[Azure Stream Analytics](https://docs.microsoft.com/en-us/azure/stream-analytics/?WT.mc_id=article-github-dglover) enables you to gain real-time insights in to your device, sensor, infrastructure, and application data.

See the [Visualizing IoT Data](http://thinglabs.io/workshop/cs/nightlight/visualize-iot-with-powerbi/) lab.  Replace the query in that lab with the following and be sure to change the time zone to your local time zone offset.  Australia (AEDST) is currently +11 hours.

    SELECT
        iothub.connectiondeviceid deviceid,
        Geo AS GeoLocation,
        Max(DateAdd(Hour, 10, EventEnqueuedUtcTime)) AS TimeCreated, -- AU EST UTC + 10
        Avg(Celsius) AS Temperature,
        AVG(Humidity) AS Humidity,
        AVG(Light) AS Light,
        AVG(HPa) AS AirPressure
    INTO
        [PowerBI]
    FROM
        [TelemetryHUB] TIMESTAMP BY EventEnqueuedUtcTime
    GROUP BY
        iothub.connectiondeviceid, Geo,
        TumblingWindow(minute, 30)
 
### Power BI

[Microsoft Power BI](https://powerbi.microsoft.com?WT.mc_id=article-github-dglover) makes it easy to visualise, organize and better understand your data.

Follow the notes in the See the [Visualizing IoT Data](http://thinglabs.io/workshop/cs/nightlight/visualize-iot-with-powerbi/) lab and modify the real time report as per this image.

### Power BI Designer Setup

![Power BI Designer Setup](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/PowerBIDesigner.JPG)

### Power BI Report Viewer

View on the web or with the Power BI apps available on iOS, Android and Windows.

![Power BI Report Viewer](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/PowerBIReport.JPG)

## Data Schema

The AzureClient sketch streams data in the following JSON format, of course you can change this:)

    {"Utc":"2017-01-12T08:00:38","Celsius":25.00,"Humidity":50.00,"hPa":1000,"Light":0,"Geo":"sydney","Schema":1,"Mem":21368,"Id":2}

## ESP8266 Based Development Boards

There are a number of ESP8266 based development boards available so be sure to check out this great article 
["Comparison of ESP8266 NodeMCU development boards"](http://frightanic.com/iot/comparison-of-esp8266-nodemcu-development-boards/).

## NodeMCU V2 with BMP180 Sensor

1. [NodeMCU v2](http://tronixlabs.com.au/wireless/esp8266/nodemcu-v2-lua-based-esp8266-development-kit)
2. [BMP180 Barometric Pressure Sensor](http://tronixlabs.com.au/sensors/altitude/bmp180-barometric-pressure-sensor-board/)
3. 1 x [400 Tie Point Interlocking Solderless Breadboard](http://tronixlabs.com.au/nodebots/400-tie-point-interlocking-solderless-breadboard-australia/)
4. Wires

![schematic](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/NodeMCU%20MQTT%20Board_bb.jpg)

## WeMos D1 Mini MCU

### WeMos D1 Mini with BMP180 Sensor

1. [WeMos D1 Mini](http://www.wemos.cc/)
2. [BMP180 Barometric Pressure Sensor](http://tronixlabs.com.au/sensors/altitude/bmp180-barometric-pressure-sensor-board/)
3. 1 x Mini Breadboard
4. Wires

![WeMos D1 Mini BMP180](https://raw.githubusercontent.com/gloveboxes/Arduino-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/WeMosD1Mini_bb.jpg)

### WeMos D1 Mini with DHT Shield Sensor

No wiring required, just solder the supplied header pins for the WeMos and the DHT Sensor shield.

1. [WeMos D1 Mini](http://www.wemos.cc/)
2. [DHT Shield](http://www.wemos.cc/wiki/doku.php?id=en:dht) or the [DHT Pro Shield](http://www.wemos.cc/wiki/doku.php?id=en:dht_pro).

![WeMos D1 Mini](https://raw.githubusercontent.com/gloveboxes/Arduino-NodeMCU-ESP8266-Secure-Azure-IoT-Hub-Client/master/AzureClient/Fritzing/WeMosD1MiniDHTShield.jpg)

## Software Requirements

## Drivers

1. NodeMCU - On Windows, Mac and Linux you will need to install the latest [CP210x USB to UART Bridge VCP Drivers](https://www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx).
2. WeMos - On Windows and Mac install the latest [Ch340G drivers](http://www.wemos.cc/Tutorial/get_started_in_arduino.html). No drivers required for Linux.
3. [ESP8266 Thing Development Board Hookup Guide](https://learn.sparkfun.com/tutorials/esp8266-thing-development-board-hookup-guide/hardware-setup)

## Arduino IDE

As at November Jan 2018 use:-

1. [Arduino IDE 1.8.7](https://www.arduino.cc/en/Main/Software).
2. ESP8266 Board Manager **2.3.0** for HTTPS/TLS Secure Client support.

## Arduino Boards Manager ESP8266 Support

Arduino version 1.6.4 and above allows installation of third-party platform packages using Boards Manager.

1. Install Arduino 1.8.7 from the Arduino website.
2. Start Arduino and open Preferences window.
3. Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json  into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
4. Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).
5. Select NodeMCU or WeMos D1 Mini Board: Tools -> Board -> NodeMCU 1.0 (ESP-12E module) or WeMos D1 Mini
6. Set Port and Upload Speed: Tools.  Note, you may need to try different port speeds to successfully flash the device. Faster is better as each time you upload the code to your device you are re-flashing the complete ROM not just your code.

## ESP8266 Arduino Core Documentation 

Be sure to read the [ESP8266 Arduino Core Documentation](http://esp8266.github.io/Arduino/versions/2.0.0/) - there are some minor gotchas.
