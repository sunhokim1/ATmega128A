/*
 * 05.ULTRASONIC.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#define LED_TIME 20
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수
#include <stdio.h>


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
	
	DDRA = 0x01;
	
	int dim = 0;
	int direction = 1;

	
	sei(); // 전역(대문) interrupt 허용
    while (1) {
		turn_on_LED_in_PWM_manner(dim);
		
		dim += direction;
		if (dim == 0) direction = 1;
		if (dim == 255) direction = -1;
    }
}



void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}

void turn_on_LED_in_PWM_manner(int dim) {
	int i;
	
	PORTA = 0xff;
	
	for(i=0;i<256;i++) {
		if (i > dim) PORTA = 0x00;
		_delay_us(LED_TIME);
	}
}