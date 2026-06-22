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
volatile 
/*
ISR (Interrupt Service Routine) : 인터럽트 처리 함수 ISR로 시작
TIMER0_OVF_vect : Timer 0 Overflow INT가 발생이 되면 이곳으로 진입한다.
250개의 펄스를 count(1ms) 하면 이곳으로 자동진입한다.
ISR은 가능한 짧게 작성한다.
*/
extern uint32_t sec_count;
volatile int ms = 0;
volatile int is_auto = 0;
extern uint32_t dot_display;

ISR(TIMER0_OVF_vect) {
	TCNT0 = 6;
	  if(is_auto)
	  {
		  ms_count++;

		  if(1000 < ms_count)
		  {
			  sec_count++;
			  ms_count=0;
			  dot_display = !dot_display;
		  }

	  }
}

void init_timer0(void);

extern void init_led(void);
extern int led_main(void);
extern void init_ultrasonic(void);
extern void led_flower_off(void);
extern void ultrasonic_processing(volatile int *flag, volatile int pin);
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
extern volatile int flag_l;
extern volatile int flag_c;
extern volatile int flag_r;
extern volatile uint16_t ultrasonic_distance_l;
extern volatile uint16_t ultrasonic_distance_c;
extern volatile uint16_t ultrasonic_distance_r;
int is_block_left = 0;
int is_block_center = 0;
int is_block_right = 0;
extern void init_fnd(void);
extern void init_timer1_pwm(void);
extern void init_button(void);
extern void init_motor_driver(void);
void manual_mode(void);
void auto_mode(void);
void auto_mode_check(void);
void distance_check(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);


int func_state = MANUAL_MODE;
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
	printf("left, center, right : %d, %d, %d\n", is_block_left, is_block_center, is_block_right);
	if (is_block_left == 1 && is_block_center == 1 && is_block_right == 0)
		turn_right(700);
	else if (is_block_left == 0 && is_block_center == 1 && is_block_right == 1)
		turn_left(700);
	else if (is_block_center == 0)
		forward(500);
	else if (is_block_center == 1)
		backward(500);
	
}

void auto_mode_check(void) {

}

void distance_calc(int i) {
	if (i == 0)
		ultrasonic_processing(&flag_l, TRIG_PIN_L);
	else if (i == 1)
		ultrasonic_processing(&flag_c, TRIG_PIN_C);
	else if (i == 2)
		ultrasonic_processing(&flag_r, TRIG_PIN_R);
}

void distance_check(void) {
	is_block_left   = (ultrasonic_distance_l <= 20);
	is_block_center = (ultrasonic_distance_c <= 20);
	is_block_right  = (ultrasonic_distance_r <= 20);
}



int main(void) {
	init_uart0();
	stdout = &OUTPUT;
	init_led();
	init_button();
	init_timer0();
	init_fnd();
	init_uart1();
	init_ultrasonic();
	init_timer1_pwm();
	init_motor_driver();
	

	
	sei(); // 전역(대문) interrupt 허용
	int i = 0;
    while (1) {
		if (get_button(BUTTON0, BUTTON0PIN)) {
			func_state = !func_state;
			if (func_state == MANUAL_MODE)
				stop();
		}
		if (func_state == AUTO_MODE) {
			is_auto = 1;
			distance_calc(i);
			distance_check();
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

void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}
