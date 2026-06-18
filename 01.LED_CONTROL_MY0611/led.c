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
	static int i=0;
	
	*(unsigned char *)0x3b |= 0x01 << i;
	_delay_ms(30);
	i = (i + 1) % 8;
	if (i == 0)
		*(unsigned char *)0x3b = 0x01;
}

void led_shift_right_keep_on(void) {
	static int i=0;
	
	*(unsigned char *)0x3b |= 0x80 >> i;
	_delay_ms(30);
	i = (i + 1) % 8;
	if (i == 0)
		*(unsigned char *)0x3b = 0x80;
}

void led_flower_on(void) {
	static int i=0;
	
	*(unsigned char *)0x3b |= (0x18 << i) | (0x18 >> i);
	_delay_ms(150);
	i = (i + 1) % 4;
	if (i == 0)
		*(unsigned char *)0x3b = 0x18;
}

void led_flower_off(void) {
	static int i=0;
	
	if (i == 0)
		*(unsigned char *)0x3b = 0xff;
	else
		*(unsigned char *)0x3b &= 0xff & ~((0x01 << (i - 1)) | (0x80 >> (i - 1)));
	
	_delay_ms(150);
	i = (i + 1) % 5;
	if (i == 0)
		*(unsigned char *)0x3b = 0xff;
}