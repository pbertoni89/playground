#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#define MYU32

#ifdef MYU32
	typedef uint32_t myt;
#else
	typedef uint8_t myt;
#endif

/**
 * Format an integer number to binary mode. Assumes little endian
 * https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
 * @param sz
 * @param ptr
 */
void print_bits(size_t const sz, myt v)
{
	unsigned char * b = (unsigned char*) &v;
	unsigned char byte;
	int i, j;

	for (i = sz - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			if (sz == 1 && j == 3) printf(" ");
			printf("%u", byte);
		}
		printf(" ");
	}
}


void print_word(const char * descr, myt w)
{
#ifdef MYU32
	printf("%6s  0x%08X  ", descr, w);
#else
	printf("%6s  0x%02X  ", descr, w);
#endif
	print_bits(sizeof(myt), w);
}


/**
 * @param w - input word to be reversed
 */
myt word_flip_i_surrender(myt w)
{
	myt r = w & 1;                       // r will be reversed bits of w; first get LSB of w
	int s = sizeof(w) * CHAR_BIT - 1;    // extra shift needed at end. CHAR_BIT comes from <limits.h>

	for (w >>= 1; w; w >>= 1)
	{
		r <<= 1;
		r |= w & 1;
		s--;
	}
													print_word("rbef", r); printf("     (s %d)\n", s);
	r <<= s;                             // shift when w's highest bits are zero
													print_word("raft", r); printf("\n");
	return r;
}


myt word_flip_ultimate(myt w)
{
	unsigned int NO_OF_BITS = sizeof(w) * 8;
	unsigned int i, temp;
	myt r = 0;

	for (i = 0; i < NO_OF_BITS; i++)
	{
		temp = (w & (1 << i));
		if (temp)
			r |= (1 << ((NO_OF_BITS - 1) - i));
	}
	return r;
}


myt word_flip(myt w)
{
	myt r = w;
	const size_t SZB = 8 * sizeof(myt);
													printf("\n"); print_word(">w", w); printf("\n");
	for (int i = 0; i < SZB; i++)
	{
		const int j = SZB - i - 1;
		const uint32_t m = (1 << i) | (1 << j);
		r = r ^ m;
													printf("%2u, %2u\n", i, j);
													print_word("r", r); printf("        ");
													print_word("m", m); printf("\n");
	}
	// const myt partl = (r & 0x00FF), partr = (r & 0xFF00);
	//myt partl = (r & 0xF0), partr = (r & 0x0F);
	//												print_word("partl", partl); printf("\n");
	//												print_word("partr", partr); printf("\n");
	//partl >>= 4;
	//partr <<= 4;      // TODO check it. was 8 
	//												print_word("partl", partl); printf("\n");
	//												print_word("partr", partr); printf("\n");
	//r = partl | partr;
	//												print_word("f", r); printf("\n");
	return r;
}

// #define MYFUN word_flip
// #define MYFUN word_flip_i_surrender
#define MYFUN word_flip_ultimate

void word_flip_test(const myt WL, const myt WR)
{
													printf("\n\n\n");
													print_word("@WL", WL);
													printf("\n");
													print_word("#WR", WR);
													printf("\n");
													printf("\ntest r to l\n");
	const myt WRF = MYFUN(WR);
													printf("\ntest l to r\n");
	const myt WLF = MYFUN(WL);

	const bool err1 = (WL != WRF), err2 = (WR != WLF);
	if (err1)
		printf("!!!!!! WL != WRF\n");
	if (err2)
		printf("!!!!!! WR != WLF\n");
	if (!err1 && !err2)
		printf("=)))))) SUCCESS\n");
}


void demo_flip()
{
#ifdef MYU32
	// word_flip_test(0b1000000010000000010000000010000, 0b0000100000000100000000100000001);
	word_flip_test(0xFF000000, 0x000000FF);
	word_flip_test(0x00000000, 0x00000000);
	word_flip_test(0x0000A000, 0x000A0000);
	word_flip_test(0x01234567, 0xe6a2c480);
#else
	word_flip_test(0b10000010, 0b01000001);
	word_flip_test(0b11001010, 0b01010011);
	word_flip_test(0b10101010, 0b01010101);
#endif
}



/*
	write dword 0: 0xFF000000	=	11111111000000000000000000000000
	write dword 1: 0x00000000	=	00000000000000000000000000000000
	write dword 2: 0x00000000	=	00000000000000000000000000000000
	write dword 3: 0x00000000	=	00000000000000000000000000000000
	write dword 4: 0x00000000	=	00000000000000000000000000000000
	write dword 5: 0x00000000	=	00000000000000000000000000000000
	read  dword 0: 0x0000007F mismatch
	read  dword 5: 0x00080000 mismatch

	e' al contrario e c'è un warp around di un bit cioe' l'ultimo (ovvero quell'8 che vedi nella word 5) va per primo.
	Scrivi il vettore come

	word 0         179, 31                      0
	word 1         63                           32
	...
	word 5                        178           0
*/
void demo_fix()
{
	uint32_t wf = 0x0000007F, wl = 0x00080000;
	printf("wf 0x%08X\t", wf);
	print_bits(sizeof(uint32_t), wf);
	printf("wl 0x%08X\t", wl);
	print_bits(sizeof(uint32_t), wl);
	printf("\n");

	wf = wf << 24;
	wf |= wl << 12;
	wl &= ~wl;

	printf("wf 0x%08X\t", wf);
	print_bits(sizeof(uint32_t), wf);
	printf("wl 0x%08X\t", wl);
	print_bits(sizeof(uint32_t), wl);
	printf("\n");
}


void main()
{
	demo_flip();
	// demo_fix();  // OK, tested
}
