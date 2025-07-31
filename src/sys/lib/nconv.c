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
    number /= 10;
    *str = (char)(number % 10) + '0';
    str--;
    rank--;
  }
}

