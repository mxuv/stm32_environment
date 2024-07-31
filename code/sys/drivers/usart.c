#include <stdint.h>

#include "usart.h"
#include "usart_hal.h"

#ifdef USART_EN
#ifdef USART_RX_EN
static uint8_t usart_InBuf[USART_InbufL];
static uint8_t usart_InBufIndexFirst;
static uint8_t usart_InBufIndexLast;
#endif

#ifdef USART_TX_EN
static uint8_t usart_OutBuf[USART_OUTBUF_SIZE];
static uint8_t usart_OutBufIndexFirst;
static uint8_t usart_OutBufIndexLast;
#endif
uint8_t usart_State;

void usart_reset_index(void)
{
#ifdef USART_RX_EN
	usart_InBufIndexFirst = 0;
	usart_InBufIndexLast = 0;
#endif
#ifdef USART_TX_EN
	usart_OutBufIndexFirst = 0;
	usart_OutBufIndexLast = 0;
#endif
}

void USART1_IRQHandler(void)
{
#ifdef USART_TX_EN
	if (((USART->ISR & USART_TXE) == USART_TXE) && (USART->CR1 & USART_TXEIE))
	{
		if (usart_State & USART_OUT_BUFER_EMPTY)
		{
			usart_interrupt_disable(USART_TXEIE);
			return;
		}
		usart_State &= (~USART_OUT_BUFER_FULL);
		USART->TDR = usart_OutBuf[usart_OutBufIndexFirst];
		usart_OutBufIndexFirst++;
		if (usart_OutBufIndexFirst == (USART_OUTBUF_SIZE - 1))
			usart_OutBufIndexFirst = 0;
		if (usart_OutBufIndexFirst == usart_OutBufIndexLast)
		{
			usart_State |= USART_OUT_BUFER_EMPTY;
			usart_interrupt_disable(USART_TXEIE);
		}
		return;
	}
#endif

#ifdef USART_RX_EN
	if (((USART->ISR & USART_RXNE) == USART_RXNE)
			&& (USART->CR1 & USART_RXNEIE))
	{
		if (usart_State & USART_IN_BUFER_FULL)
		{
			volatile uint8_t tmp = (uint8_t) USART->RDR;
			((void)tmp);
			return;
		}
		usart_State &= (~USART_IN_BUFER_EMPTY);
		usart_InBuf[usart_InBufIndexLast] = (uint8_t) USART->RDR;
		usart_InBufIndexLast++;
		if (usart_InBufIndexLast == (USART_InbufL - 1))
			usart_InBufIndexLast = 0;
		if (usart_InBufIndexLast == usart_InBufIndexFirst)
			usart_State |= USART_IN_BUFER_FULL;
	}
#endif
}

#ifdef USART_TX_EN
void usart_outbuf_push(uint8_t data)
{
	if (usart_State & USART_OUT_BUFER_FULL)
		return;
	usart_interrupt_disable(USART_TXEIE);
	usart_OutBuf[usart_OutBufIndexLast] = data;
	usart_OutBufIndexLast++;
	usart_State &= (~USART_OUT_BUFER_EMPTY);
	if (usart_OutBufIndexLast == (USART_OUTBUF_SIZE - 1))
		usart_OutBufIndexLast = 0;
	if (usart_OutBufIndexLast == usart_OutBufIndexFirst)
		usart_State |= USART_OUT_BUFER_FULL;
	usart_interrupt_enable(USART_TXEIE);
}

void usart_send_string(const char *string)
{
  while (*string != '\0')
  {
    usart_outbuf_push(*string);
    string++;
  }
}
#endif

#ifdef USART_RX_EN
uint8_t usart_inbuf_pop(void)
{
	uint8_t temp;

	usart_interrupt_disable(USART_RXNEIE);
	usart_State &= (~USART_IN_BUFER_FULL);
	temp = usart_InBuf[usart_InBufIndexFirst];
	usart_InBufIndexFirst++;
	if (usart_InBufIndexFirst == (USART_InbufL - 1))
		usart_InBufIndexFirst = 0;
	if (usart_InBufIndexFirst == usart_InBufIndexLast)
		usart_State |= USART_IN_BUFER_EMPTY;
	usart_interrupt_enable(USART_RXNEIE);
	return temp;
}
#endif

#endif
