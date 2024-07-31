#include <stdint.h>

#include "onewire_config.h"
#include "one_wire.h"
#include "mcu.h"
#include "tim.h"
#include "gpio.h"
#include "gpio_config.h"

#ifdef ONEWIRE_EN
uint8_t onewire_buffer[9] = {0};

void onewire_tx0(void);
void onewire_tx1(void);
uint8_t onewire_rx(void);

void onewire_tx0(void)
{
	GPIO_PIN_RESET(OW_PORT, OW_PIN); 		/* ����� � 0 */
	tim_delay_us(OW_TIMER, OW_TIME_W0);
	GPIO_PIN_SET(OW_PORT, OW_PIN); 			/* ����� � 1 */
	tim_delay_us(OW_TIMER, OW_TIME_REC);
}

void onewire_tx1(void)
{
	GPIO_PIN_RESET(OW_PORT, OW_PIN); 		/* ����� � 0 */
	tim_delay_us(OW_TIMER, OW_TIME_W1);
	GPIO_PIN_SET(OW_PORT, OW_PIN); 			/* ����� � 1 */
	tim_delay_us(OW_TIMER, OW_TIME_REC + OW_TIME_SLOT - OW_TIME_W1);
}

void onewire_txbyte(uint8_t dat)
{
	uint8_t tmp = 1;

	for (uint8_t i = 0; i < 8; i++)
	{
		if (tmp & dat)
			onewire_tx1();
		else
			onewire_tx0();

		tmp = tmp << 1;
	}
}

uint8_t onewire_rx(void)
{
	GPIO_PIN_RESET(OW_PORT, OW_PIN); 		/* ����� � 0 */
	tim_delay_us(OW_TIMER, OW_TIME_W1);
	GPIO_PIN_SET(OW_PORT, OW_PIN);			/* ����� � 1 */
	GPIO_SET_MODE(OW_PORT, GPIO_MODE_IN, OW_PIN_CONF);

	if (GPIO_GET_PIN(OW_PORT, OW_PIN) == 0)
	{
		tim_delay_us(OW_TIMER, OW_TIME_R0);
		if (GPIO_GET_PIN(OW_PORT, OW_PIN) == 0)
		{
			tim_delay_us(OW_TIMER, OW_TIME_REC + OW_TIME_SLOT - (OW_TIME_R0 * 2));
			GPIO_SET_MODE(OW_PORT, GPIO_MODE_OUT, OW_PIN_CONF);
			return 0;
		}
		else
		{
			tim_delay_us(OW_TIMER, OW_TIME_REC);
			GPIO_SET_MODE(OW_PORT, GPIO_MODE_OUT, OW_PIN_CONF);
			return 1;
		}
	}
	else
	{
		tim_delay_us(OW_TIMER, OW_TIME_REC);
		GPIO_SET_MODE(OW_PORT, GPIO_MODE_OUT, OW_PIN_CONF);
		return 1;
	}
}

uint8_t onewire_rxbyte(void)
{
	uint8_t tmp = 1;
	uint8_t val = 0;

	for (uint8_t i = 0; i < 8; i++)
	{
		if (onewire_rx())
			val |= tmp;
		tmp = tmp << 1;
	}
	return val;
}

uint8_t onewire_reset(void)
{
	uint8_t tmp;

	GPIO_PIN_RESET(OW_PORT, OW_PIN); 							/* ����� � 0 */
	tim_delay_us(OW_TIMER, OW_TIME_RESET);
	GPIO_PIN_SET(OW_PORT, OW_PIN); 								/* ����� � 1 */
	GPIO_SET_MODE(OW_PORT, GPIO_MODE_IN, OW_PIN_CONF);
	tim_delay_us(OW_TIMER, OW_TIME_REC); 							/* ���� ����� �������������� */

	if (GPIO_GET_PIN(OW_PORT, OW_PIN) == 0) 	/* ����� � ����, ��� ��*/
	{
		tim_delay_us(OW_TIMER, OW_TIME_PRESENCE); 			/* ����� ������ */
		if (GPIO_GET_PIN(OW_PORT, OW_PIN))
			tmp = 0; 																	/* �� ����� 1, ��� ��. ���������� ��������� ����� */
		else
			tmp = OW_ERR_BF; 													/* ����� � 0, �� ��� ������������� ���������� */
	}
	else 																					/* ����� � 1 */
	{
		tim_delay_us(OW_TIMER, OW_TIME_PRESENCE / 2); 	/* �������� �������� ������������ ������ */
		if (GPIO_GET_PIN(OW_PORT, OW_PIN)) 			/* ����� � 1, ���������� �� �������� */
			tmp = OW_ERR_NA;
		else
		{
			tim_delay_us(OW_TIMER, OW_TIME_PRESENCE); 		/* ����� � 0, ��� ��, �������� ����� ������ � �������� ����� */
			if (GPIO_GET_PIN(OW_PORT, OW_PIN)) 		/* �� ����� 1, ��� ��. ���������� ��������� ����� */
				tmp = 0;
			else
				tmp = OW_ERR_BF;
		}
	}
	tim_delay_us(OW_TIMER, OW_TIME_PRESENCE);
	GPIO_SET_MODE(OW_PORT, GPIO_MODE_OUT, OW_PIN_CONF);

	return tmp;
}

void onewire_init(void)
{
  gpio_pin_config(OW_PORT, GPIO_MODE_OUT, GPIO_OUT_OD, GPIO_PULL_UP,
      GPIO_SPEED_HIGH, OW_PIN_CONF);
  GPIO_PIN_SET(OW_PORT, OW_PIN);
}
#endif

