#include "TempSensorNTC.h"

const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 1000;             // 1k ohm series resistor
const double adc_resolution = 4095; // 12-bit adc

const double A = 0.001129148; // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741;

TempSensorNTC::TempSensorNTC()
{
}

void TempSensorNTC::setup()
{
  pinMode(pinT1, INPUT);
  pinMode(pinT2, INPUT);
}

bool TempSensorNTC::handle()
{
  skipStepsCounter++;
  if (skipSteps < skipStepsCounter)
  {
    skipStepsCounter = 0;
    tempValues1 += analogRead(pinT1);
    tempValues2 += analogRead(pinT2);
    samplesCollected++;
    if (samplesCollected == sumValuesCount)
    {
      status.tempm1 = calculate(tempValues1 / sumValuesCount);
      status.tempm2 = calculate(tempValues2 / sumValuesCount);
      samplesCollected = 0;
      tempValues1 = 0;
      tempValues2 = 0;
      return true;
    }
  }
  return false;
}

double TempSensorNTC::calculate(int adc_value)
{
  double Vout, Rth, temperature;

  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

  /*  Steinhart-Hart Thermistor Equation:
      Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
      where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)), 3)))); // Temperature in kelvin
  temperature = temperature - 273.15;                                  // Temperature in degree celsius

  return temperature;
}
