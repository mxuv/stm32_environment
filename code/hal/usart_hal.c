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

/* ��� ���������������� USART:
 * 	�������� ������������ USART
 * 	�������� ������������ �����, ������ �������� ����� �������������� USART-��
 * 	������� ����� ������ ����� ��� ���� �������
 * 	��� Tx ���� ���������� ��� ������ ������
 * 	��� Rx ���� ���������� �������� � ����� ��� ��� ��������
 * 	������ �������� ������ ����� ��� �������
 *	������ ����� ���� ������� (��. �������, GPIO)
 *	��������� ������������ ��������
 *	�������� �����, ��������, ����������
 *	�������� �������� ���������� USART
 */

/*������������ USART1 ��� ������� USART ���������� �� �����������*/

void usart_init(void)
{
	RCC_CLKEN_USART1(); /*��� ������������ UART*/

	USART_PORT->AFR[0] |= USART_GPIO_AF << USART_TX_AF_POS; /* | USART_GPIO_AF << USART_RX_AF_POS; */
	gpio_pin_config(USART_PORT, GPIO_MODE_AF, GPIO_OUT_PP, GPIO_PULL_NO, GPIO_SPEED_HIGH, USART_TX_PIN_CONF);
/*
  GPIO_ConfigurePin(USART_PORT, GPIO_MODE_AF, GPIO_OUT_PP, GPIO_PULL_UP, GPIO_SPEED_HIGH, USART_RX_PIN_CONF);
*/

	USART->BRR = USART_BAUDDIV;    /*������������*/
	USART->CR1 |= USART_CR1_TE |   /* ��� ����������� */
	           /* USART_CR1_RE |      ��� ��������� */
	              USART_CR1_UE;    /* ��� USART */
	            /* USART_CR1_RXNEIE;  ��� ���������� �� ������ ����� */
	NVIC_EnableIRQ(USART1_IRQn);    /* ��� ���������� �� USART*/

	usart_reset_index();
	usart_State = USART_IN_BUFER_EMPTY | USART_OUT_BUFER_EMPTY;
}
