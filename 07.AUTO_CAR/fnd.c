/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:46:18
 *  Author: kccistc
 */ 

#include "fnd.h"
#include <avr/interrupt.h>	

void init_fnd(void);
void find_display(uint32_t sec_count, uint32_t dot_display);
int fnd_main(void);
void display_min_sec_dot(void);
void min_sec_clock();
void sec_clock();
void find_sec_clock(uint32_t sec_count, uint32_t dot_display);
void stopwatch_clock();
void run_stop_watch(uint32_t sec_count, uint32_t ms_count);	// PC7번이 1초 단위로 ON/OFF되도록

void pause_stop_watch();
void reset_stop_watch();

void time_set_fnd(int min_time);
void fnd_all_off(void);

int wash_running_express_fnd(int wash_time);

extern volatile uint32_t ms_count;
uint32_t sec_count = 0;	
uint32_t dot_display = 0;

void min_sec_clock()
{	
	find_display(sec_count, dot_display);
}


void init_fnd(void)
{
	1ST_FND_DIGIT_DDR = 0xff;	// 출력 모드로 설정
	1ST_FND_DIGIT_DDR |= 1 << 1ST_FND_DIGIT_D1 | 1 << 1ST_FND_DIGIT_D2 | 1 << 1ST_FND_DIGIT_D3 | 1 << 1ST_FND_DIGIT_D4;
	
	// FND 전체 off 하는 작업
#if 0	// Common Anode (양극)
	1ST_FND_DATA_PORT = 0xff;

#else	// Common Cathode (음극)
	1ST_FND_DATA_PORT = ~0xff;

#endif
}

void find_display_1st(uint32_t sec_count, uint32_t dot_display)	// PC7번이 1초 단위로 ON/OFF되도록
{
	//  0      1 	 2	   3	 4	   5 	 6 	      7      8	    9
	uint8_t find_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x98, ~0x7f};	// common cathode

	static int digit_select = 0;	// 자리수 선택

	switch(digit_select) {
		case 0:
		1ST_FND_DIGIT_PORT = 0xff;	// 이 과정이 없으면 잔사잉 많이 나타남..
		1ST_FND_DATA_PORT = find_font[sec_count % 10] | (0x80 * dot_display);
		1ST_FND_DIGIT_PORT = (uint8_t)~0x80;
		break;

		case 1:
		1ST_FND_DIGIT_PORT = 0xff;
		1ST_FND_DATA_PORT = find_font[sec_count / 10 % 6] | (0x80 * dot_display);
		1ST_FND_DIGIT_PORT = (uint8_t)~0x40;
		break;

		case 2:
		1ST_FND_DIGIT_PORT = 0xff;
		1ST_FND_DATA_PORT = find_font[sec_count / 60 % 10];
		1ST_FND_DIGIT_PORT = (uint8_t)~0x20;
		break;

		case 3:
		1ST_FND_DIGIT_PORT = 0xff;
		1ST_FND_DATA_PORT = find_font[sec_count / 600 % 6];
		1ST_FND_DIGIT_PORT = (uint8_t)~0x10;
		break;
	}

	digit_select = (digit_select + 1 ) % 4;
}


void find_display_2nd(uint32_t sec_count, uint32_t dot_display)	// PC7번이 1초 단위로 ON/OFF되도록
{
	//  0      1 	 2	   3	 4	   5 	 6 	      7      8	    9
	uint8_t find_font[] = {~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x98, ~0x7f};	// common cathode

	static int digit_select = 0;	// 자리수 선택

	switch(digit_select) {
		case 0:
		FND_DIGIT_PORT = 0xff;	// 이 과정이 없으면 잔사잉 많이 나타남..
		FND_DATA_PORT = find_font[sec_count % 10] | (0x80 * dot_display);
		FND_DIGIT_PORT = (uint8_t)~0x80;
		break;

		case 1:
		FND_DIGIT_PORT = 0xff;
		FND_DATA_PORT = find_font[sec_count / 10 % 6] | (0x80 * dot_display);
		FND_DIGIT_PORT = (uint8_t)~0x40;
		break;

		case 2:
		FND_DIGIT_PORT = 0xff;
		FND_DATA_PORT = find_font[sec_count / 60 % 10];
		FND_DIGIT_PORT = (uint8_t)~0x20;
		break;

		case 3:
		FND_DIGIT_PORT = 0xff;
		FND_DATA_PORT = find_font[sec_count / 600 % 6];
		FND_DIGIT_PORT = (uint8_t)~0x10;
		break;
	}

	digit_select = (digit_select + 1 ) % 4;
}