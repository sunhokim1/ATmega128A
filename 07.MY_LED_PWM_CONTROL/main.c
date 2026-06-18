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
#if 0
	init_led();
	init_timer0();
	
	DDRA = 0x01;
	
	int dim = 0;
	int direction = 1;
	
	// 파형 출력 핀인 PB4 핀(OC0핀)을 출력으로 설정
	DDRB |= (1 << PB4);
	
	// 타이머/카운터 0번을 고속 PWM모드로 설정
	TCCR0 |= (1 << WGM01) | (1 << WGM00);
	// 비반전모드
	TCCR0 |= (1<< COM01);
	//분주비를 1024로 설정
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	//sei(); // 전역(대문) interrupt 허용
    while (1) {
		OCR0 = dim;
		_delay_ms(10);
		
		dim += direction;
		
		if (dim == 0) direction = 1;
		if (dim == 255) direction = -1;
    }
	#else
	// 파형 출력 핀인 PB5(OC1A핀), PB6(OC1B핀)을 출력으로 설정
	DDRB |= (1<< PB5) | (1<< PB6);
	
	// 타이머/카운터 1번을 8비트 위상 교정 PWM 모드로 설정
	TCCR1A |= (1 << WGM10);
	
	// 비교 일치 A는 비반전모드, 비교일치 B는 반전모드로 설정
	#endif
	return 0;
}


void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT
}

//void turn_on_LED_in_PWM_manner(int dim) {
	//int i;
	//
	//PORTA = 0xff;
	//
	//for(i=0;i<256;i++) {
		//if (i > dim) PORTA = 0x00;
		//_delay_us(LED_TIME);
	//}
//}