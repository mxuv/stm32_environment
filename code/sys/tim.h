#ifndef __TIM_H__
#define __TIM_H__

#include "mcu.h"

#define TIM_COUNT_ENABLE(TIMx)					(TIMx->CR1 |= TIM_CR1_CEN)
#define TIM_COUNT_DISABLE(TIMx)					(TIMx->CR1 &= ~TIM_CR1_CEN)

#define TIM_SET_PRESCALER(TIMx, Val)		(TIMx->PSC = Val)

#define TIM_SET_ARR(TIMx, Val)					(TIMx->ARR = Val)
#define TIM_GET_ARR(TIMx)               (TIMx->ARR)

#define TIM_SET_CCR1(TIMx, Val)         (TIMx->CCR1 = Val)
#define TIM_GET_CCR1(TIMx)              (TIMx->CCR1)

#define TIM_UPDATE_INT_EN(TIMx)					(TIMx->DIER |= TIM_DIER_UIE)

#define TIM_CLEAR_UI_FLAG(TIMx)				  (TIMx->SR &= ~TIM_SR_UIF)

#define TIM_SET_UPDATE_EVENT(TIMx)			(TIMx->EGR |= TIM_EGR_UG)

void tim_delay_us(TIM_TypeDef *TIMx, uint16_t delay);

#endif
