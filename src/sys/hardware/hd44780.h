#ifndef HD44780_H
#define HD44780_H

#define HD44780_F_ID						1 << 1					//����� ������ �����/������ 1/0
#define HD44780_F_S							1 							//����� ��������/��������		1/0
#define HD44780_F_D							1 << 2					//������� �����������
#define HD44780_F_C							1 << 1					//������ � ���� ������������� 1/0
#define HD44780_F_B							1								//������ � ���� ��������� ����� 1/0
#define HD44780_F_SC						1 << 3					//����� ������/������� 1/0
#define HD44780_F_RL						1 << 2					//����� ������ �����/������ 1/0
#define HD44780_F_DL						1 << 4					//������ ���� 8 ���/ 4 ���� 1/0
#define HD44780_F_N							1 << 3					//��� ������/���� ������ 1/0
#define HD44780_F_F							1 << 2					//������ ������� 5�10/5�8 1/0
#define HD44780_TX_COM					0
#define HD44780_TX_DAT					1

#define HD44780_DISPLAY_CLEAR		0b00000001
#define HD44780_DDRAM						0b10000000

#define HD44780_8B_2L_5X8				(1 << 5 | HD44780_F_DL | HD44780_F_F | HD44780_F_N)
#define HD44780_4B_2L_5X8				(1 << 5 | HD44780_F_F | HD44780_F_N)

#define HD44780_DISPLAY_OFF			(1 << 3)
#define HD44780_DISPLAY_ON			(1 << 3 | HD44780_F_D)
#define HD44780_CURSOR_EN				(1 << 3 | HD44780_F_D | HD44780_F_C)
#define HD44780_CURSOR_BLINK		(1 << 3 | HD44780_F_D | HD44780_F_C | HD44780_F_B)
#define HD44780_CUR_SHIFT_EN_R	(1 << 2 | HD44780_F_ID)

#endif
