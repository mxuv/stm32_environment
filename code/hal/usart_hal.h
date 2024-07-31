#ifndef USART_HAL_H
#define USART_HAL_H

#include "mcu.h"
#include "usart_config.h"

#define USART_TXEIE				USART_CR1_TXEIE
#define USART_TXE					USART_ISR_TXE
#define USART_RXNEIE			USART_CR1_RXNEIE
#define USART_RXNE				USART_ISR_RXNE

void usart_interrupt_enable(uint32_t irq);
void usart_interrupt_disable(uint32_t irq);
void usart_init(void);

#endif
