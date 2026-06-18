/*
 * button.c
 *
 * Created: 2026-06-10 오후 1:25:29
 *  Author: kccistc
 */ 

#include "button.h"

void init_button(void);
int get_button(int button_num, int button_pin);
void (*fp_button0[])();
void (*fp_button1[])();
void fp_button(void (*fp[])(), int *button, int active_button);
void reset_button(int *button);

extern void init_led(void);
extern void led_all_on(void);
extern void led_all_off(void);
extern void led_right_on(void);
extern void led_left_on(void);
extern void led_odd_on(void);
extern void led_even_on(void);
//버튼 초기화 방향설정(입력)

void init_button(void) {
	BUTTON_DDR &= ~(1 << BUTTON0PIN
					| 1 << BUTTON1PIN
					| 1 << BUTTON2PIN
					| 1 << BUTTON3PIN);
}

// 예) BUTTON0 3
// button을 눌렀다 떼면 : 1을 리턴
// button이 idle 상태 : 0을 리턴

int get_button(int button_num, int button_pin) {
	static unsigned char button_status[BUTTON_NUMBER] = {
		BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE
	};
	
	int current_state;
	//1. button을 읽는다.
	current_state = BUTTON_PIN & (1 << button_pin);
	//2. 버튼상태 체크
	if (current_state && button_status[button_num] == BUTTON_RELEASE) {  // 버튼이 처음 눌러진 상태
		_delay_ms(15); // noise가 지나가기를 기다린다. 
		button_status[button_num] = BUTTON_PRESS;
		return 0; //아직은 완전히눌렀다 뗀 상태가 아니다.
	}else if (button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE) { // 버튼이 이전에 눌러진 상태였으며 지금은 떼어진 상태
		_delay_ms(15);
		button_status[button_num] = BUTTON_RELEASE; // 다음 버튼을 체크하기 위해서 초기화
		return 1;
	}
	
	return 0;
}

void (*fp_button0[])() = {
	led_all_off,
	led_all_on,
	led_right_on,
	led_left_on,
};

void (*fp_button1[])() = {
	led_all_off,
	led_odd_on,
	led_even_on,
	led_all_on,
};

void fp_button(void (*fp[])(), int *button, int active_button) {
	for (int i=0; i<BUTTON_NUMBER; i++) {
		if (i != active_button)
		button[i] = 0;
	}
	button[active_button]++;
	if (button[active_button] == 4)
	button[active_button] = 0;
	fp[button[active_button]]();
}

void reset_button(int *button) {
	for (int i=0;i<BUTTON_NUMBER;i++) {
		button[i] = 0;
	}
	led_all_off();
}