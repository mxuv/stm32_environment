#include "nconv.h"

uint8_t mrank(uint32_t *number, uint32_t rank)
{
	uint8_t ret_val = 0;
	while (*number >= rank)
	{
		ret_val++;
		*number -= rank;
	}
	return ret_val;
}

void uint_str(uint8_t digits, uint32_t number, uint8_t *str)
{
  uint32_t rank = 1000000000;

  switch(digits)
  {
  case 8:
    rank = 100;
    break;
  case 16:
    rank = 10000;
    break;
  case 32:
  default:
    rank = 1000000000;
    break;
  }

/*  for (uint8_t i = 0; i < 10; i++) */
  while (rank)
  {
    if (number >= rank)
    {
      *str = mrank(&number, rank) + 0x30;
      str++;
    }
    else
    {
      *str = 0x30;
      str++;
    }
    rank = rank / 10;
  }
}
/*
void uint32_str(uint32_t number, uint8_t *str)
{
  uint32_t rank = 1000000000;

  for (uint8_t i = 0; i < 10; i++)
  {
    if (number >= rank)
    {
      *str = mrank(&number, rank) + 0x30;
      str++;
    }
    else
    {
      *str = 0x30;
      str++;
    }
    rank = rank / 10;
  }
}*/
