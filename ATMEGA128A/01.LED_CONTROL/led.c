/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keep_on(void);
void led_shift_right_keep_on(void);
void led_flower_on(void);
void led_flower_off(void);
int led_main(void);

int led_main(void) {
	led_all_off();
	while(1) {
		led_flower_off();
	}
}

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

void led_shift_left_on(void) {
#if 1
	static int i=0;
	
	*(unsigned char *)0x3b = 0x01 << i;
	_delay_ms(30);
	
	i = (i + 1) % 8;
#else
	for (int i=0;i<8;i++) {	// 2400ms소요 이따위로 짜면 안된다.
		//PORTA = (1 << i); // PORTA : 0x1B
		*(unsigned char *)0x3b = 0x01 << i;
		_delay_ms(30);
	}
#endif
}

void led_shift_right_on(void) {
#if 1
	static int i=0;
	
	*(unsigned char *)0x3b = 0x80 >> i;
	_delay_ms(30);
	
	i = (i + 1) % 8;
#else
		for (int i=0;i<8;i++) {
			//PORTA = (PORTA >> 1); // PORTA : 0x1B
			*(unsigned char *)0x3b = 0x80 >> i;
			_delay_ms(30);
		}
#endif
}

void led_shift_left_keep_on(void) {
	PORTA = 0x00;
	for (int i = 0;i<8;i++) {
		PORTA |= (1 << i);
		
		_delay_ms(300);
	}
}

void led_shift_right_keep_on(void) {
	PORTA = (1 << 7);
	_delay_ms(300);
	for (int i=0;i<7;i++) {
		PORTA |= (PORTA >> 1);
		_delay_ms(300);
	}
}

void led_flower_on(void) {
	PORTA = (1<<3) | (1<<4);
	_delay_ms(300);
	for (int i=0;i<3;i++) {
		PORTA |= (PORTA << 1) | (PORTA >> 1);
		_delay_ms(300);
	}
}

void led_flower_off(void) {
	led_all_on();
	_delay_ms(300);
	for (int i=0;i<4;i++) {
		PORTA &= ~((1<<i) | (1 << (7 - i)));
		_delay_ms(300);
	}
}