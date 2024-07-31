#ifndef __USART_H__
#define __USART_H__

#include "usart_config.h"

extern uint8_t usart_State;

#define USART_IN_BUFER_EMPTY  	0b00000001
#define USART_IN_BUFER_FULL   	0b00000010
#define USART_OUT_BUFER_EMPTY 	0b00000100
#define USART_OUT_BUFER_FULL  	0b00001000

#define USART_IN_BUFER_NOTEMPTY	!(USART_State & USART_IN_BUFER_EMPTY)

void USART1_IRQHandler(void);
void usart_reset_index(void);

#ifdef USART_TX_EN
void usart_outbuf_push(uint8_t data);
void usart_send_string(const char *string);
#endif
#ifdef USART_RX_EN
uint8_t usart_inbuf_pop(void);
#endif

#endif
