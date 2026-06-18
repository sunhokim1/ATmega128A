/*
 * 01.LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:21
 * Author : kccistc
 */ 
#if 1

#define F_CPU 16000000UL // 16MHz
#include <avr/io.h> // PORTA PORTD 등의 I/O register들이 들어있다.
#include <util/delay.h> // _delay_ms _delay_us 등의 함수가 들어있다.
#include "button.h"
#include "led.h"

extern void init_led(void); // init_led함수는 다른 파일에 들어있다고 compiler 신고
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void (*fp_button0[])();
extern void (*fp_button1[])();
extern void fp_button(void (*fp[])(), int *button, int active_button);
extern void reset_button(int *button);

int main(void) {
	int button[BUTTON_NUMBER] = {0};
	init_button();
	init_led();
	
	while(1) {
		if (get_button(BUTTON0, BUTTON0PIN)) {
			fp_button(fp_button0, button, 0);
		}
		if (get_button(BUTTON1, BUTTON1PIN)) {
			fp_button(fp_button1, button, 1);
		}
		if(get_button(BUTTON3, BUTTON3PIN)) {
			reset_button(button);
		}
		
	}
}


#endif
