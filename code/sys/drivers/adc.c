#include <stdint.h>
#include "adc.h"

uint16_t vref_intcal;

void adc_disable(ADC_TypeDef *ADCx)
{
  ADC_STOP(ADCx);
  while (ADC_STOPPED(ADCx) != 0)
  {

  }
  ADC_DISABLE(ADCx);
  while (ADC_ENABLED(ADCx) != 0)
  {

  }
}

void adc_enable(ADC_TypeDef *ADCx)
{
  if (ADC_RDY(ADCx) != 0)
  {
    ADC_RDY_CLEAR(ADCx);
  }
  ADC_ENABLE(ADCx);
  while (ADC_RDY(ADCx) == 0)
  {

  }
}

void adc_calibrate(ADC_TypeDef *ADCx)
{
  vref_intcal = *((uint16_t*) VREF_INTCAL_ADDR);

  if (ADC_ENABLED(ADCx) != 0)
  {
    ADC_DISABLE(ADCx);
  }
  while (ADC_ENABLED(ADCx) != 0)
  {

  }
  ADC_DMA_DISABLE(ADCx);
  ADC_CALIBRATE(ADCx);
  while (ADC_IS_CALIBRATET(ADCx) != 0)
  {

  }
}
