#include <stdio.h>
#include <stdint.h>


uint8_t ByteSpecular(uint8_t b)
{
	int i, bit;
	char s = 0;
	for (i=0; i<8; i++)
	{
		if ((b >> i) & 0x1)
		{
			s |= (0x1 << (8 - i - 1));
		}
	}
	return s;
}


void main()
{
	char t1 = 3, t2 = 250;
	printf("%d = BS(%d)\n", ByteSpecular(t1), t1);
	printf("%d = BS(%d)\n", ByteSpecular(t2), t2);
}
