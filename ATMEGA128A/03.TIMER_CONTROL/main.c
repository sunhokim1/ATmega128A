/*
 * 03.TIMER_CONTROL.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수

volatile uint32_t ms_count = 0; // volatile 최적화 방지
/*
ISR (Interrupt Service Routine) : 인터럽트 처리 함수 ISR로 시작
TIMER0_OVF_vect : Timer 0 Overflow INT가 발생이 되면 이곳으로 진입한다.
250개의 펄스를 count(1ms) 하면 이곳으로 자동진입한다.
ISR은 가능한 짧게 작성한다.
*/

ISR(TIMER0_OVF_vect) {
	TCNT0 = 6;
	ms_count++; // 1ms count
}

void init_timer0(void);

extern void init_led(void);
extern int led_main(void);

int main(void)
{
	init_timer0();
	init_led();
	led_main();
    while (1) {
		
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
	sei(); // 전역(대문) interrupt 허용
}