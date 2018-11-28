#ifndef _SASTOKEN_H
#define _SASTOKEN_H

#include "Base64.h"
#include "sha256.h"
#include <TimeLib.h> // http://playground.arduino.cc/code/time - installed via library manager

#define MAX_HOST_DEVICE_LEN
#define BUFSIZE 150
#define SAS_TOKEN_SIZE 300

class SasToken
{
public:
  SasToken()
  {
  }

protected:
  bool generateSasToken();
  void tokeniseConnectionString(char *cs);

  struct security
  {
    time_t expiryPeriodInSeconds = 60 * 60; // Default to 60 minutes
    time_t expiryTime = 0;
    char token[SAS_TOKEN_SIZE];
  } sas;

  struct config
  {
    char *host;
    char *id;
    char *key;
  } device;

protected:
  int urlEncode(char *dest, char *msg);

private:
  char buff[BUFSIZE];
  virtual void createSasToken(char *key);
  char *getValue(char *token, char *key);
};

#endif // _SASTOKEN
