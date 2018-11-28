#include "IotHub.h"

bool IotHub::verifyServerFingerprint()
{
#ifdef ARDUINO_ARCH_ESP8266
  if (strlen(certificateFingerprint) == 0)
  {
    return true;
  }

  if (client->verify(certificateFingerprint, device.host))
  {
    Serial.print("Certificate fingerprint verified against ");
    Serial.print(device.host);
    Serial.println(" successfully");
  }
  else
  {
    Serial.println("Certificate fingerprint verification failed");
    ESP.restart();
  }
#endif
  return true;
}

int IotHub::publishData(char *data, int dataLength)
{
  int startChar = 0;
  int len = 0;
  int bytesWritten = 0;

  while (startChar < dataLength) // write out data in chunks
  {
    yield();
    len = (startChar + SEGMENT_LENGTH < dataLength) ? SEGMENT_LENGTH : dataLength - startChar;
    strncpy(buff, data + startChar, len);
    buff[len] = '\0';
    bytesWritten += client->print(buff);
    startChar += len;
  }

  return bytesWritten;
}

bool IotHub::connect()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }

  if (!client->connected())
  {
    if (!client->connect(device.host, 443))
    {
      Serial.print("Host connection failed.  WiFi IP Address: ");
      Serial.println(WiFi.localIP());
      return false; //"Not Connected";
    }
    else
    {
      if (!verifyServerFingerprint())
      {
        return false; // "Cert Fingerprint does not validate";
      }
    }
  }

  return client->connected();
}

bool IotHub::publishBegin(int dataLength)
{
  if (!connect())
  {
    return false;
  }

  generateSasToken();

  int postLen = snprintf(buff, BUFSIZE, httpRequest, endPoint, device.host, sas.token, dataLength);

  yield();
  client->flush();

  int result = client->print(buff); //write out http header

  return postLen == result;
}

int IotHub::publishEnd()
{
  String response = "";
  int retry = 0;
  int result = -1;

  yield();

  if (client->connected())
  {
    client->setTimeout(2000);
    delay(40);

    response = client->readStringUntil('\n'); // first line has the HTTP response code

    client->setTimeout(1000);

    while (client->connected() && retry++ < 20)
    { // ignore rest of the header info
      delay(40);
      String line = client->readStringUntil('\n');
      if (line == "\r") // headers received
      {
        break;
      }
    }
  }

  if (response.startsWith("HTTP"))
  {
    result = response.substring(9, 12).toInt();
  }

  if (result == -1)
  {
    delay(500);
    client->flush();
  }

  return result;
}

int IotHub::publish(char *data)
{
  int dataLength = strlen(data);
  if (!publishBegin(dataLength))
  {
    client->flush();
    return 400;
  }
  int bytesSent = publishData(data, dataLength);
  if (bytesSent != dataLength)
  {
    client->flush();
    return 400;
  }
  return publishEnd();
}