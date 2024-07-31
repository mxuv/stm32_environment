#ifndef __ONE_WIRE_H__
#define __ONE_WIRE_H__

#define OW_TIME_RESET		  500	/* ������������ ������� �����, ��� */
#define OW_TIME_PRESENCE	240 /* ������������ ������ "�����������" */
#define OW_TIME_REC			  60	/* ������������ �������� ����� ��������, 15-60 ��� */
#define OW_TIME_SLOT			120	/* ������������ ������ ������, 60-120 ��� */
#define OW_TIME_W1				2		/* ������������ ������ ��� ������ 1, ��� */
#define OW_TIME_W0				80	/* ������������ ������ ��� ������ 1, ��� */
#define OW_TIME_R0				10	/* ������������ ������ ��� ������ 0, ��� */

#define OW_ERR_BF				0b00000001
#define OW_ERR_NA				0b00000010
#define OW_ERR_MSK			0b00000011

extern uint8_t onewire_buffer[9];

void onewire_tx0(void);
void onewire_tx1(void);
uint8_t onewire_rx(void);
uint8_t onewire_reset(void);
uint8_t onewire_rxbyte(void);
void onewire_txbyte(uint8_t dat);
void onewire_init(void);

#endif
