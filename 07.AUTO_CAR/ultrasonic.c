/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:32:46
 *  Author: kccistc
 */ 
#include "ultrasonic.h"

extern volatile int ultrasonic_check_time;


extern volatile int ultrasonic_distance_l;
extern volatile int ultrasonic_distance_c;
extern volatile int ultrasonic_distance_r;

void init_ultrasonic(void); 
void make_trigger(volatile int pin);
void ultrasonic_processing_l(void);
void ultrasonic_processing_c(void);
void ultrasonic_processing_r(void);

extern volatile int flag_l;
extern volatile int flag_c;
extern volatile int flag_r;

volatile uint16_t start_time_l = 0;
volatile uint16_t start_time_c = 0;
volatile uint16_t start_time_r = 0;


// p278 표12-3 참조
// INT4 : PE4 외부 INT4 초음파 센서 상승/하강에지 발생시, 이곳으로 들어옴
// 결국, 상승 에지때 1번, 하강 에지때 1번 들어옴
// 0x000A 
ISR (INT4_vect)
{
	// 1. 상승에지
	if(ECHO_PORT & (1 << ECHO_PIN_L))
	{
		start_time_l = TCNT3;
	}
	else // 2. 하강에지
	{
		uint16_t clc_time = TCNT3 - start_time_l;	
		ultrasonic_distance_l = clc_time * 64;
		flag_l = 0;

	}
}

ISR(INT5_vect)
{
	// 1. 상승에지
	if(ECHO_PORT & (1 << ECHO_PIN_C))
	{
		start_time_c = TCNT3;
	}
	else // 2. 하강에지
	{
		uint16_t clc_time = TCNT3 - start_time_c;
		ultrasonic_distance_c = clc_time * 64;
		flag_c = 0; 

	}
}

ISR(INT6_vect)
{
	// 1. 상승에지
	if(ECHO_PORT & (1 << ECHO_PIN_R))
	{
		start_time_r = TCNT3;
	}
	else // 2. 하강에지
	{
		uint16_t clc_time = TCNT3 - start_time_r;
		ultrasonic_distance_r = clc_time * 64;	// QnA: 바로 58로 나눠도 같은 결과 나지 않나..? cm로 주는?
		flag_r = 0;

	}
}

void init_ultrasonic(void)
{
	TRIG_DDR |= (1 << TRIG_PIN_L) | (1 << TRIG_PIN_C) | (1 << TRIG_PIN_R);	// output mode
	ECHO_DDR &= ~((1 << ECHO_PIN_L) | (1 << ECHO_PIN_C) | (1 << ECHO_PIN_R));
	
	EICRB |= (0 << ISC41 | 1 << ISC40 | 0 << ISC51 | 1 << ISC50 | 0 << ISC61 | 1 << ISC60);
	
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;

    TCCR3B |= (1 << CS32) | (1 << CS30);

	// ----- EINT4 설정 -----
	// page 247 그림 12-6
	EIMSK |= 1 << INT4 | 1 << INT5 | 1 << INT6;	// 외부 INT4(ECHO핀 설정)


}

void make_trigger(volatile int pin) 
{
	TRIG_PORT &= ~(1 << pin); // low로 만든다.
	_delay_us(1);
	TRIG_PORT |= 1 << pin; // high로 만든다.
	_delay_us(15);    //규격에는 10us인데 redunacy로 15us
	TRIG_PORT &= ~(1 << pin);
}

void ultrasonic_processing_l(void)
{
	if(flag_l == 0){
		flag_l = 1;
		make_trigger(TRIG_PIN_L);
	}
}

void ultrasonic_processing_c(void)
{
	if(flag_c == 0){
		flag_c = 1;
		make_trigger(TRIG_PIN_C);
	}
}

void ultrasonic_processing_r(void)
{
	if(flag_r == 0){
		flag_r = 1;
		make_trigger(TRIG_PIN_R);
	}
}