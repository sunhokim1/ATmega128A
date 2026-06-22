/*
 * washing_machine.c
 *
 * Created: 2026-06-18 오후 4:14:25
 *  Author: kccistc
 */ 


#include "washing_machine.h"
#include "led.h"
#include "button.h"
#include "ultrasonic.h"
#include "fnd.h"

extern int get_button(int button_num, int button_pin);
extern void led_all_off(void);
extern void fnd_display(int motor_direction);
extern volatile uint32_t ms_count;
extern uint32_t washing_time;
extern uint32_t rinse_time;
extern uint32_t spin_time;
extern volatile uint32_t wt_count;
extern volatile int run;
extern int motor_direction;


int current_setting_mode = READY_MODE;
int current_run_mode = READY_MODE;
int motor_direction_duration = 0;

void led_wash_on(void);
void led_rinse_on(void);
void led_spin_on(void);
void led_ready(void);
void run_machine();
void play_washing(int mode);
void washing_time_setting();
extern void fnd_set_time_display();


void (*fp_mode[])() = {
	led_ready,
	led_wash_on,
	led_rinse_on,
	led_spin_on,
};

void washing_machine_main(void) {
	if (run == STOP) {
		fnd_set_time_display();
		led_ready();
	}
	if (run == RUN) {
		run_machine();
		if(run) OCR3C = 250;
		else OCR3C = 0;		
	}
	if (get_button(BUTTON0, BUTTON0PIN)) {
		if (current_run_mode != READY_MODE) {
			run = STOP;
			washing_time = 0;
			rinse_time = 0;
			spin_time = 0;
			wt_count = 0;
			OCR3C = 0;
			current_setting_mode = READY_MODE;
			current_run_mode = READY_MODE;
		}else if (current_setting_mode == READY_MODE) {
			run = RUN;
			current_run_mode = WASHING_MODE;
		}
	}
	
	if (get_button(BUTTON1, BUTTON1PIN) && run == STOP)
		current_setting_mode = (current_setting_mode + 1) % 4;
	if (get_button(BUTTON2, BUTTON2PIN) && run == STOP) {
		washing_time_setting();
	}
	
}


void run_machine() {
	if (current_run_mode == WASHING_MODE) {
		play_washing(WASHING_MODE);
		led_wash_on();
		if (washing_time <= 0)
			current_run_mode = RINSE_MODE;
	}else if(current_run_mode == RINSE_MODE) {
		play_washing(RINSE_MODE);
		led_rinse_on();
		if (rinse_time <= 0)
			current_run_mode = SPIN_MODE;
	}else if(current_run_mode == SPIN_MODE) {
		play_washing(SPIN_MODE);
		led_spin_on();
		if (spin_time <= 0)
			current_run_mode = READY_MODE;
	}else if (current_run_mode == READY_MODE) {
		run = STOP;
		led_ready();
		current_setting_mode = READY_MODE;
	}
}
void washing_time_setting() {
	if (current_setting_mode == WASHING_MODE)
		washing_time += 60;
	else if (current_setting_mode == RINSE_MODE)
		rinse_time += 60;
	else if (current_setting_mode == SPIN_MODE)
		spin_time += 60;
}

void play_washing(int mode) {
	fnd_display(motor_direction);
	if (wt_count >= 1000) {
		wt_count = 0;
		motor_direction_duration++;
		if (mode == WASHING_MODE) washing_time--;
		else if(mode == RINSE_MODE) rinse_time--;
		else if(mode == SPIN_MODE) spin_time--;
		
		motor_direction_duration = (motor_direction_duration + 1) % 5;
		if (motor_direction_duration == 0) {
			motor_direction = !motor_direction;
			PORTF &= ~(1 << 6 | 1 << 7); // reset
			if (motor_direction) {
				PORTF |= 1 << 6; // 정회전
			}else PORTF |= 1 << 7; // 역회전
		}
	}
}

void led_ready(void) {
	led_all_off();
}

// LED_세탁모드
void led_wash_on(void) {
	led_all_off();
	*(unsigned char *)0x3b = 0x01;
}

// LED_헹굼모드
void led_rinse_on(void) {
	led_all_off();
	*(unsigned char *)0x3b = 0x01 << 1;
}

// LED_탈수모드
void led_spin_on(void) {
	led_all_off();
	*(unsigned char *)0x3b = 0x01 << 2;
}