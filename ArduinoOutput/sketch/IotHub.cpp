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
    Serial.println(" sucessfully");
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
  int bytesWritten;

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

int IotHub::publishBegin(int dataLength)
{

  if (WiFi.status() != WL_CONNECTED)
  {
    return -1;
  }

  if (!client->connected())
  {
    if (!client->connect(device.host, 443))
    {
      Serial.print("Host connection failed.  WiFi IP Address: ");
      Serial.println(WiFi.localIP());
      return -3; //"Not Connected";
    }
    else
    {
      if (!verifyServerFingerprint())
      {
        return -4; // "Cert Fingerprint does not validate";
      }
    }
  }

  if (!client->connected())
  {
    return -2;
  }

  generateSasToken();

  int postLen = snprintf(buff, BUFSIZE, httpRequest, endPoint, device.host, sas.token, dataLength);

  Serial.println(buff);

  yield();
  client->flush();

  return client->print(buff); //write out http header
}

int IotHub::publishEnd()
{
  String response = "";
  String chunk = "";
  int limit = 0;
  int result = -1;

  yield();
  client->setTimeout(2000);

  do
  {
    if (limit == 1)
    {
      client->setTimeout(250); // once got first chunk of response reduce timeout
    }
    if (client->connected())
    {
      delay(40);
      chunk = client->readStringUntil('\n');
      response += chunk;
    }
  } while (chunk.length() > 0 && ++limit < 100);

  if (response.length() > 12)
  {
    if (response.startsWith("HTTP"))
    {
      result = response.substring(9, 12).toInt();
    }
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
  publishBegin(dataLength);
  publishData(data, dataLength);
  return publishEnd();
}

void IotHub::tokeniseConnectionString(char *cs)
{
  char *value;
  char *pch = strtok(cs, ";");

  while (pch != NULL)
  {
    value = getValue(pch, "HostName");
    if (NULL != value)
    {
      this->device.host = value;
    }
    value = getValue(pch, "DeviceId");
    if (NULL != value)
    {
      this->device.id = value;
    }
    value = getValue(pch, "SharedAccessKey");
    if (NULL != value)
    {
      this->device.key = value;
    }
    pch = strtok(NULL, ";");
  }
}

char *IotHub::getValue(char *token, char *key)
{
  int valuelen;
  int keyLen = strlen(key) + 1; // plus 1 for = char

  if (NULL == strstr(token, key))
  {
    return NULL;
  }

  valuelen = strlen(token + keyLen);
  char *arr = (char *)malloc(valuelen + 1);
  strcpy(arr, token + keyLen);
  return arr;
}
