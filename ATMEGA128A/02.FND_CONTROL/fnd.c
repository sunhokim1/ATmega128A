/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:46
 *  Author: kccistc
 */ 

#include "fnd.h"

void init_fnd(void);
void fnd_display(void);

uint32_t ms_count = 0; //ms를 재는 count uint32_t : unsigned int
uint32_t sec_count = 0; //sec를 재는 count uint32_t : unsigned int 
uint8_t	 dot_display = 0;

int fnd_main(void) {
	init_fnd();
	
	while(1) {
		fnd_display();
		_delay_ms(1);
		ms_count++;
		if (ms_count >= 1000) { // 1000ms --> 1sec
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
	
	return 0;
}

void init_fnd(void) {
	FND_DATA_DDR = 0xff; //출력모드로 설정한다.
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;

	FND_DATA_PORT = (uint8_t)~0xff;

	
}

void fnd_display(void) {
						// 0	 1	   2	  3	   4	 5	   6	 7	   8	  9		.
	uint8_t fnd_font[] = { (uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};

	static int digit_select = 0; //자리 수 선택
	
	switch(digit_select) {
		case 0 :        
			FND_DIGIT_PORT = (uint8_t)~0x80; //1단위
			FND_DATA_PORT = fnd_font[sec_count%10]; // 0~9 까지 출력
		break;
		case 1 :	//10단위
			FND_DIGIT_PORT = ~0x40;
			FND_DATA_PORT = fnd_font[sec_count/10%6];
		break;
		case 2 :	//100단위
			FND_DIGIT_PORT = ~0x20;
			FND_DATA_PORT = fnd_font[sec_count/60%10];
		break;
		case 3 :	//1000단위
			FND_DIGIT_PORT = ~0x10;
			FND_DATA_PORT = fnd_font[sec_count/600%6];
		break;
	}
	digit_select = (digit_select + 1) % 4;
}

