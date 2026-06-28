/*
 * 05.ULTRASONIC.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수
#include <stdio.h>
#include "define.h"
#include "button.h"
#include "ultrasonic.h"


volatile uint32_t ms_count = 0; // volatile 최적화 방지
volatile uint32_t sec_count = 0;
volatile int is_auto = 0;
volatile uint8_t dot_display = 0;
volatile uint32_t wt_count = 0;
int func_state = MANUAL_MODE;

extern void fnd_display();
void init_timer0(void);

extern void init_led(void);
extern int led_main(void);
extern void init_ultrasonic(void);
extern void led_flower_off(void);
extern void ultrasonic_processing_l(void);
extern void ultrasonic_processing_c(void);
extern void ultrasonic_processing_r(void);
extern void make_trigger(volatile int pin);
extern void init_uart0(void);
extern void init_uart1(void);
extern void UART0_transmit(void);
extern int ultrasonic_distance;
extern void led_distance_change_on(void);
extern volatile uint8_t bt_data;
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);
extern void stop(void);
volatile int flag_l = 0;
volatile int flag_c = 0;
volatile int flag_r = 0;
volatile uint16_t ultrasonic_distance_l = 0;
volatile uint16_t ultrasonic_distance_c = 0;
volatile uint16_t ultrasonic_distance_r = 0;
int is_block_left = 0;
int is_block_center = 0;
int is_block_right = 0;
extern void init_fnd(void);
extern void init_timer1_pwm(void);
extern void init_button(void);
extern void init_motor_driver(void);
extern int get_button(int button_num, int button_pin);
void manual_mode(void);
void auto_mode(void);
void auto_mode_check(void);
void distance_check(int i);
void play_clock(void);
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

ISR(TIMER0_OVF_vect) {
	TCNT0 = 6;
	ms_count++;
	wt_count++;
}


void (*pfunc[])()= {
	manual_mode,
	auto_mode,
	auto_mode_check,
	distance_check,
};


void manual_mode(void){
	switch(bt_data) {
		case 'F':
		case 'f':
			forward(500); // 4us * 500 = 0.002sec(2ms) 
			break;
		case 'B':
		case 'b':
			backward(500);
			break;
		case 'L':
		case 'l':
			turn_left(700);
			break;
		case 'R':
		case 'r':
			turn_right(700);
			break;
		case 'S':
		case 's':
			stop();
			break;
		default:
			break;
	}
}

void auto_mode(void) {
	if(is_block_left == 1 && is_block_center == 1 && is_block_right == 1)
		backward(500);
	else if (is_block_center == 1) {
		if (ultrasonic_distance_l >= ultrasonic_distance_r)
			turn_left(700);
		else if (ultrasonic_distance_l < ultrasonic_distance_r)
			turn_right(700);
	}else if (is_block_center == 0)
		forward(500);
}

void auto_mode_check(void) {

}

void distance_calc(int i) {
	if (i == 0)
		ultrasonic_processing_l();
	else if (i == 1)
		ultrasonic_processing_c();
	else if (i == 2)
		ultrasonic_processing_r();
}

void distance_check(int i)
{
	 printf("L=%d C=%d R=%d\r\n",
	 ultrasonic_distance_l / 58,
	 ultrasonic_distance_c / 58,
	 ultrasonic_distance_r / 58);
	 
	if (i == 0 )
		is_block_left = (ultrasonic_distance_l <= 15 * 58);

	else if (i == 1)
		is_block_center = (ultrasonic_distance_c <= 30 * 58);

	else if (i == 2)
		is_block_right = (ultrasonic_distance_r <= 15 * 58);
}



int main(void) {
	init_uart0();
	init_uart1();
	init_led();
	init_button();
	init_timer0();
	init_fnd();
	init_ultrasonic();
	init_timer1_pwm();
	init_motor_driver();

stdout = &OUTPUT;
	sei(); // 전역(대문) interrupt 허용
	int i = 0;
	
    while (1) {
		fnd_display();
		if (get_button(BUTTON0, BUTTON0PIN)) {
			func_state = !func_state;
			if (func_state == MANUAL_MODE) {
				stop();
			}
		}
		if (func_state == AUTO_MODE) {
			
			 distance_calc(i);
			 distance_check(i);
			i = (i + 1) % 3;
		}
		pfunc[func_state]();
    }
}

/*
	1. timer0을 초기화 한다.
	AVR에서 8bit timer 0 / 2 중에서 0번을 초기화한다.
	임베디드 쪽에서 가장 신경을 써야 할 부분이 초기화 하는 것이다.
	초기화가 잘못 되면 이후가 다 꼬인다. 
	2. 8bit가지고 1ms를 측정하는 timer / counter를 만들고자 한다.
	2.1 분주기 설정 (64분주)
		16000000MHz / 64 ==> 250,000MHz
	2.2 1주기가 잡아먹는 시간 계산
	T = 1/f = 1/250000 ==> 0.000004sec (4us) ==> 0.004ms
	2.3 8bit 가지고 count 하는 시간을 계산 (8bit timer OV(Overflow))
	0.004MHz * 256 ==> 0.001024sec ==> 1.024ms
	0.004MHz * 250 ==> 0.001sec ==> 1ms
*/

void play_clock()
{
	
	
}

void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}
