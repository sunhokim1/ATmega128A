/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:46
 *  Author: kccistc
 */ 

#include "fnd.h"

void init_fnd(void);
void fnd_display(int mode);

extern volatile uint32_t ms_count; //ms를 재는 count uint32_t : unsigned int
extern volatile uint32_t sec_count; //sec를 재는 count uint32_t : unsigned int 
extern volatile uint8_t	 dot_display;
extern volatile uint32_t wt_count;

void init_fnd(void) {
	FND_DATA_DDR = 0xff; //출력모드로 설정한다.
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;

	FND_DATA_PORT = (uint8_t)~0xff;

}


void fnd_display(int mode) {
						// 0	 1	   2	  3	   4	 5	   6	 7	   8	  9		.
	uint8_t fnd_font[] = { (uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};
	static int circle = 0;

	static int digit_select = 0; //자리 수 선택
	
	
	if (mode == 0) {
		switch(digit_select) {
			case 0:
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1:
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2:
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count / 60 % 10] ^ (0x80 * dot_display);
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3:
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count / 600 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}else if (mode == 1) {
		switch(digit_select) {
			case 0 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count / 10 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 3)
			FND_DATA_PORT = 0x08;
			else if (circle == 4)
			FND_DATA_PORT = 0x04;
			else if (circle == 5)
			FND_DATA_PORT = 0x02;
			else if (circle == 6)
			FND_DATA_PORT = 0x01;
			else
			FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 0)
			FND_DATA_PORT = 0x20;
			else if (circle == 1)
			FND_DATA_PORT = 0x10;
			else if (circle == 2)
			FND_DATA_PORT = 0x08;
			else if (circle == 7)
			FND_DATA_PORT = 0x01;
			else
			FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}else if (mode == 2) {
		switch(digit_select) {
			case 0:   // 제일 오른쪽: 1/100초 1의 자리
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[(wt_count / 10) % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1:   // 오른쪽 두 번째: 1/100초 10의 자리
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[(wt_count / 100) % 10];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2:   // 왼쪽 두 번째: 초 1의 자리
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[sec_count % 10];
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3:   // 제일 왼쪽: 초 10의 자리
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[(sec_count / 10) % 6];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}
	
	digit_select = (digit_select + 1) % 4;
	circle = sec_count % 8;
}

