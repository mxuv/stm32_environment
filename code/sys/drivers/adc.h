#ifndef __ADC_H__
#define __ADC_H__

#include "mcu.h"

#define ADC_ENABLE(ADCx)            (ADCx->CR |= ADC_CR_ADEN)
#define ADC_ENABLED(ADCx)           (ADCx->CR & ADC_CR_ADEN)

#define ADC_DISABLE(ADCx)           (ADCx->CR |= ADC_CR_ADDIS)

#define ADC_DMA_ENABLE(ADCx)        (ADCx->CFGR1 |= ADC_CFGR1_DMAEN )
#define ADC_DMA_ENABLE_CIRC(ADCx)   (ADCx->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG)
#define ADC_DMA_DISABLE(ADCx)       (ADCx->CFGR1 &= ~ADC_CFGR1_DMAEN)

#define ADC_CALIBRATE(ADCx)         (ADCx->CR |= ADC_CR_ADCAL)
#define ADC_IS_CALIBRATET(ADCx)     (ADCx->CR & ADC_CR_ADCAL)

#define ADC_END_CONV(ADCx)          (ADCx->ISR & ADC_ISR_EOC)
#define ADC_RDY(ADCx)               (ADCx->ISR & ADC_ISR_ADRDY)
#define ADC_RDY_CLEAR(ADCx)         (ADCx->ISR |= ADC_ISR_ADRDY)

#define ADC_START(ADCx)             (ADCx->CR |= ADC_CR_ADSTART)

#define ADC_STOP(ADCx)              (ADCx->CR |= ADC_CR_ADSTP)
#define ADC_STOPPED(ADCx)           (ADCx->CR & ADC_CR_ADSTP)

#define ADC_CONT_EN(ADCx)           (ADCx->CFGR1 |= ADC_CFGR1_CONT)
#define ADC_VREF_EN(ADCx)           (ADCx->CCR |= ADC_CCR_VREFEN)

#define ADC_CLK_MODE(ADCx, mode)    (ADCx->CFGR2 = mode)
#define ADC_CLK_DEF                 (0U)
#define ADC_CLK_PCLK_DIV_2          (ADC_CFGR2_CKMODE_0)
#define ADC_CLK_PCLK_DIV_4          (ADC_CFGR2_CKMODE_1)

#define ADC_SMPT_1_5                (0U)
#define ADC_SMPT_7_5                (ADC_SMPR_SMP_0)
#define ADC_SMPT_13_5               (ADC_SMPR_SMP_1)
#define ADC_SMPT_28_5               (ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1)
#define ADC_SMPT_41_5               (ADC_SMPR_SMP_2)
#define ADC_SMPT_55_5               (ADC_SMPR_SMP_0 | ADC_SMPR_SMP_2)
#define ADC_SMPT_71_5               (ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2)
#define ADC_SMPT_239_5              (ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2)

#define ADC_SMP_TIME(ADCx, SMPtime) (ADCx->SMPR |= SMPtime)

extern uint16_t vref_intcal;

void adc_enable(ADC_TypeDef *ADCx);
void adc_disable(ADC_TypeDef *ADCx);
void adc_calibrate(ADC_TypeDef *ADCx);

#endif
