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


#define VER_XS_PROTO   0x03u
#define VER_XS_KERNEL  0x08u
#define VER_XS_ZYNQ    0x0Cu
#define VER_XS_CNTRL    0x01u
#define VER_XS_WHOLE  VER_XS_PROTO << 24 | VER_XS_KERNEL << 16 | VER_XS_ZYNQ << 8 | VER_XS_CNTRL

void main()
{
	char t1 = 3, t2 = 250;
	printf("%d = BS(%d)\n", ByteSpecular(t1), t1);
	printf("%d = BS(%d)\n", ByteSpecular(t2), t2);

	printf("%u\n", 0x00FF0000u >> 16);
	printf("%u\n", 0xFFu >> 16);
	printf("\n");
	printf("%u\n", VER_XS_PROTO);
	printf("%u\n", VER_XS_KERNEL);
	printf("%u\n", VER_XS_ZYNQ);
	printf("%u\n", VER_XS_CNTRL);
	printf("%u\n", VER_XS_WHOLE);
	printf("%08x\n", VER_XS_WHOLE);
}
