#include "nconv.h"
#include "mdiv.h"

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

#if 1
static uint32_t getrank(uint8_t size)
{
  switch(size)
    {
    case 1:
      return 3;
      break;
    case 2:
      return 5;
      break;
    case 4:
    default:
      return 10;
      break;
    }
}

void uint_str(uint8_t size, uint32_t number, char *str)
{
  uint8_t rank;

  rank = getrank(size);
  *(str + rank) = '\0';
  str += rank - 1;
  while (rank) {
    *str = (char)(number % 10) + '0';
    number /= 10;
    str--;
    rank--;
  }
}
#else
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
#endif

