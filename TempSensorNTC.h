#ifndef TEMPSENSORNTC_H_
#define TEMPSENSORNTC_H_

#include "appconfig.h"
#include "status.h"

class TempSensorNTC
{
private:
  double calculate(int adc_value);

  const int pinT1 = 32;
  const int pinT2 = 33;
  long tempValues1;
  long tempValues2;

  int tempPotValue = 0; // used to store value read from device
  double calcValue = 0;

  const int sumValuesCount = 100; // number of samples to collect before sending average 1k = 11687 missedIntervals, 50 = 13583 missedIntervals
  long samplesCollected = 0;
  const int skipSteps = 3; // skip steps between pin value reads
  int skipStepsCounter = 0;

public:
  TempSensorNTC();
  void setup();
  bool handle();
};

#endif /* TEMPSENSORNTC_H_ */
