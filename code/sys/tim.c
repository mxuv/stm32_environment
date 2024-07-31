#include "tim.h"

void tim_delay_us(TIM_TypeDef* TIMx, uint16_t delay)
{
	TIMx->ARR = (uint32_t)delay;
	TIMx->SR = 0;
	TIM_COUNT_ENABLE(TIMx);

	while ((TIMx->SR & TIM_SR_UIF) == 0)
	{

	}

	TIM_COUNT_DISABLE(TIMx);
}
