#include "usart_config.h"
#include "usart_hal.h"
#include "usart.h"

void usart_interrupt_enable(uint32_t irq)
{
	USART->CR1 |= irq;
}

void usart_interrupt_disable(uint32_t irq)
{
	USART->CR1 &= ~irq;
}

/* Как сконфигурировать USART:
 * 	Включить тактирование USART
 * 	Включить тактирование порта, выводы которого будут использоваться USART-ом
 * 	Выбрать режим работы пинов как альт функция
 * 	для Tx пина установить тип выхода пушпул
 * 	для Rx пина установить подтяжку к плюсу или без подтяжки
 * 	задать скорость работы порта как высокую
 *	Задать номер альт функции (см. даташит, GPIO)
 *	Выставить предделитель скорости
 *	Включить прием, передачу, прерывания
 *	Включить основное прерывание USART
 */

/*Конфигурация USART1 как простой USART работающий на прерываниях*/

void usart_init(void)
{
	RCC_CLKEN_USART1(); /*Вкл тактирование UART*/

	USART_PORT->AFR[0] |= USART_GPIO_AF << USART_TX_AF_POS; /* | USART_GPIO_AF << USART_RX_AF_POS; */
	gpio_pin_config(USART_PORT, GPIO_MODE_AF, GPIO_OUT_PP, GPIO_PULL_NO, GPIO_SPEED_HIGH, USART_TX_PIN_CONF);
/*
  GPIO_ConfigurePin(USART_PORT, GPIO_MODE_AF, GPIO_OUT_PP, GPIO_PULL_UP, GPIO_SPEED_HIGH, USART_RX_PIN_CONF);
*/

	USART->BRR = USART_BAUDDIV;    /*Предделитель*/
	USART->CR1 |= USART_CR1_TE |   /* Вкл передатчика */
	           /* USART_CR1_RE |      Вкл приемника */
	              USART_CR1_UE;    /* Вкл USART */
	            /* USART_CR1_RXNEIE;  Вкл прерывания по приему байта */
	NVIC_EnableIRQ(USART1_IRQn);    /* Вкл прерываний от USART*/

	usart_reset_index();
	usart_State = USART_IN_BUFER_EMPTY | USART_OUT_BUFER_EMPTY;
}
