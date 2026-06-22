/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#define FUNC_SU 6;
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
void led_distance_change_on(void);
extern int ultrasonic_distance;
extern int func_state;
extern volatile uint32_t ms_count; // volatile 최적화 방지



void (*fp[]) () = {
	led_shift_left_on,
	led_shift_right_on,
	led_shift_left_keep_on,
	led_shift_right_keep_on,
	led_flower_on,
	led_flower_off,
};

int led_main(void) {
	uint8_t led_toggle=0;
	
	led_all_off();
	while(1) {
		fp[func_state]();
		
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
	
	if (ms_count >= 100) {
		ms_count = 0;
		*(unsigned char *)0x3b = 0x01 << i;
		
		//if ((i = (i + 1) % 8) == 0) {
			
				//func_state = (func_state + 1) % FUNC_SU;
				//led_all_off();
		//}
		i = (i + 1) % 8;
	}
	
	
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
	
	if (ms_count >= 100) {
		ms_count = 0;
		*(unsigned char *)0x3b = 0x80 >> i;
		
		
		//if ((i = (i + 1) % 8) == 0) {
			//led_all_off();
			//func_state = (func_state + 1) % FUNC_SU;
		//}
		i = (i+1) % 8;
	}
	
	
#endif
#if 0
for (int i=0;i<8;i++) {
	//PORTA = (PORTA >> 1); // PORTA : 0x1B
	*(unsigned char *)0x3b = 0x80 >> i;
	_delay_ms(30);
}
#endif
}

void led_shift_left_keep_on(void) {
	static int i = 0;
	
	if (ms_count >= 100) {
		ms_count = 0;
		*(unsigned char *)0x3b |= (1 << i);
		
		//if ((i = (i + 1) % 8) == 0) {
			//led_all_off();
			//func_state = (func_state + 1) % FUNC_SU;		
		//}
		i = (i + 1) % 8;
	}
	
}

void led_shift_right_keep_on(void) {
	static int i = 0;
	
	if (ms_count >= 100) {
		ms_count = 0;
		*(unsigned char *)0x3b |= ((0x80) >> i);
		
		//if ((i = (i + 1) % 8) == 0) {
			//led_all_off();
			//func_state = (func_state + 1) % FUNC_SU;
		//}
		i = (i + 1) % 8;
	}
	
}

void led_flower_on(void) {
	static int i = 0;
	if (i == 0)
		*(unsigned char *)0x3b = 0x00;
	
	if (ms_count >= 300) {
		ms_count = 0;
		*(unsigned char *)0x3b |= (0x10 << i) | (0x08 >> i);
		
		//if ((i = (i + 1) % 4) == 0) {
			//func_state = (func_state + 1) % FUNC_SU;
		//}
		i = (i + 1) % 4;
	}
	
}

void led_flower_off(void) {
	static int i = 0;
	if (i == 0)
		*(unsigned char *)0x3b = 0xff;
		
	if (ms_count >= 300) {
		ms_count = 0;
		*(unsigned char *)0x3b &= ~((0x80 >> i) | (0x01 << i));
		
		//if ((i = (i + 1) % 4) == 0) {
			//func_state = (func_state + 1) % FUNC_SU;
		//}
		i = (i + 1) % 4;
	}
	
}

void led_distance_change_on(void) {
	int distance = ultrasonic_distance / 58;
	
	    if (distance <= 2)
		    *(unsigned char *)0x3b = 0b00000001;
	    else if (distance <= 3)
		    *(unsigned char *)0x3b = 0b00000011;
	    else if (distance <= 4)
			*(unsigned char *)0x3b = 0b00000111;
		else if (distance <= 5)
		    *(unsigned char *)0x3b = 0b00001111;
		else if (distance <= 6)
			*(unsigned char *)0x3b = 0b00011111;
		else if (distance <= 7)
			*(unsigned char *)0x3b = 0b00111111;
		else if (distance <= 10)
			*(unsigned char *)0x3b = 0b01111111;
		else
			led_all_on();
}