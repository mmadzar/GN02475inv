#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#define HOST_NAME "GN02475inv"

#include "../secrets.h"
#include <stdint.h>
#include <Arduino.h>
#include <driver/gpio.h>
#include "shared/configtypes/configtypes.h"

#define OPMODE "opmode"
#define LASTERR "lasterr"
#define TMPHS "tmphs"
#define RPM "rpm"
#define POT "pot"
#define POT2 "pot2"
#define IL1 "il1"
#define IL2 "il2"
#define IL1RMS "il1rms"
#define IL2RMS "il2rms"
#define TMPM1 "tmpm1"
#define TMPM2 "tmpm2"

struct Settings
{
  const gpio_num_t led = (gpio_num_t)2; // status led
#define ListenChannelsCount 0
  const char *listenChannels[ListenChannelsCount] = {};

#define CollectorCount 12
  CollectorConfig collectors[CollectorCount] = {
      {OPMODE, 500},
      {LASTERR, 500},
      {TMPHS, 500},
      {RPM, 500},
      {POT, 500},
      {POT2, 500},
      {IL1, 500},
      {IL2, 500},
      {IL1RMS, 500},
      {IL2RMS, 500},
      {TMPM1, 500},
      {TMPM2, 500}};

  int getCollectorIndex(const char *name)
  {
    for (size_t i = 0; i < CollectorCount; i++)
    {
      if (strcmp(collectors[i].name, name) == 0)
        return i;
    }
    return -1;
  }
};

struct Intervals
{
  int statusPublish = 1000; // interval at which status is published to MQTT
};

extern Settings settings;
extern Intervals intervals;

#endif /* APPCONFIG_H_ */
