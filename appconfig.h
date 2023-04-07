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
#define ILMAX "ilmax"
#define ILMAXTARGET "ilmaxtarget"
#define FSLIPSPNT "fslipspnt"
#define AMP "amp"
#define IDC "idc"
#define DIN_MPROT "din_mprot"
#define DIN_OCUR "din_ocur"
#define DIN_EMCYSTOP "din_emcystop"
#define DIN_DESAT "din_desat"
#define TMPM1 "tmpm1"
#define TMPM2 "tmpm2"

struct Settings
{
  const gpio_num_t led = (gpio_num_t)2; // status led
#define ListenChannelsCount 0
  const char *listenChannels[ListenChannelsCount] = {};

#define CollectorCount 15
  CollectorConfig collectors[CollectorCount] = {
      {OPMODE, 500},
      {LASTERR, 500},
      {TMPHS, 0},
      {RPM, 0},
      {ILMAX, 0},
      {ILMAXTARGET, 0},
      {FSLIPSPNT, 0},
      {AMP, 0},
      {IDC, 0},
      {DIN_MPROT, 0},
      {DIN_OCUR, 0},
      {DIN_EMCYSTOP, 0},
      {DIN_DESAT, 0},
      {TMPM1, 1000},
      {TMPM2, 1000}};

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
