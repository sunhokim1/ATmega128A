/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:46
 *  Author: kccistc
 */ 

#include "fnd.h"

#define FND_DIGIT_MASK ((1 << FST_FND_DIGIT_D1) | (1 << FST_FND_DIGIT_D2) | (1 << FST_FND_DIGIT_D3) | (1 << FST_FND_DIGIT_D4))

void init_fnd(void);
void fnd_display();

extern volatile uint32_t ms_count; //ms를 재는 count uint32_t : unsigned int
extern volatile uint32_t sec_count; //sec를 재는 count uint32_t : unsigned int 
extern volatile uint8_t	dot_display;

void init_fnd(void) {
	FND_DATA_DDR = 0xff; //출력모드로 설정한다.
	FST_FND_DIGIT_DDR |= FND_DIGIT_MASK;
	FST_FND_DIGIT_PORT |= FND_DIGIT_MASK; 

	FND_DATA_PORT = (uint8_t)~0xff;

}


void fnd_display() {
						// 0	 1	   2	  3	   4	 5	   6	 7	   8	  9		.
	uint8_t fnd_font[] = { (uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};

	static int digit_select = 0; //자리 수 선택
	FST_FND_DIGIT_PORT |= FND_DIGIT_MASK;
	FND_DATA_PORT = 0x00;
		switch(digit_select) {
			case 0:
			FND_DATA_PORT = fnd_font[sec_count % 10];
			FST_FND_DIGIT_PORT =
			(FST_FND_DIGIT_PORT | FND_DIGIT_MASK) & ~(1 << FST_FND_DIGIT_D4);
			break;

			case 1:
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
			FST_FND_DIGIT_PORT =
			(FST_FND_DIGIT_PORT | FND_DIGIT_MASK) & ~(1 << FST_FND_DIGIT_D3);
			break;

			case 2:
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10];
			FST_FND_DIGIT_PORT =
			(FST_FND_DIGIT_PORT | FND_DIGIT_MASK) & ~(1 << FST_FND_DIGIT_D2);
			break;

			case 3:
			FND_DATA_PORT = fnd_font[sec_count / 600 % 6];
			FST_FND_DIGIT_PORT =
			(FST_FND_DIGIT_PORT | FND_DIGIT_MASK) & ~(1 << FST_FND_DIGIT_D1);
			break;
		}
	
	digit_select = (digit_select + 1) % 4;
}

