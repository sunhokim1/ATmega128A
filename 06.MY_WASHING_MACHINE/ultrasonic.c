/*
 * ultrasonic.c
 *
 * Created: 2026-06-17 오후 1:32:47
 *  Author: kccistc
 */ 

#include "ultrasonic.h"

volatile int ultrasonic_distance = 0;
volatile char scm[50];

void ultrasonic_processing(void);
void make_trigger(void);
void init_ultrasonic(void);
volatile int flag = 0;
// P278 표 12-3
// INT4 : PE4 외부 INT4 초음파 센서 상승, 하강 엣지 발생 시 이곳으로 진입
// 결국 상승에지 때 1번, 하강 에지 때 1번
// 0x000A
ISR(INT4_vect) {
	// 1. 상승 에지
	if ((ECHO_PORT & (1 << ECHO_PIN))) {
		TCNT1 = 0;
	}else{ // 2. 하강 에지
		// TCNT1 : Timer count
		// 예) TNCT1이 10들어있다고 가정
		// 15.625KHz의 1주기 : 64us
		// 0.000064sec * 10개 ==> 0.00064us(640us)
		// 640us / 58(1cm이동 하는 데 소요시간) : 11cm
		// 1sec : 1000000us
		ultrasonic_distance = TCNT1 * 1000000.0 * 1024 / F_CPU;
		sprintf(scm, "dis: %dcm\n", ultrasonic_distance / 58);
		flag = 1;
	}
}

void init_ultrasonic(void) {
	TRIG_DDR |= 1 << TRIG_PIN; // output mode로 설정
	ECHO_DDR &= ~(1 << ECHO_PIN); // input mode로 설정
	
	// P289 표 12-6 P288 그림 12-8 참조
	// 0 1 : 상승 에지 하강에지 둘다 INT를 띄우도록 요청한다.
	EICRB |= 0 << ISC41 | 1 << ISC40;
	// 16bit timer / counter 1번을 사용하기로 하자
	// timer int를 사용하지 않는다.
	// 16bit timer1 16bit로 표시할 수 있는 최대값 65535(max) : 0xffff
	// 16MHz / 1024분주  : 16000000Hz / 1024 --> 15625Hz --> 15.625KHz
	// 1주기 (1개의 필수 소요시간) : T=1/f 1/15625 --> 0.000064sec ==> 64us
	// 분주비 1024설정 P318 표 14-1
	TCCR1B |= 1 << CS12 | 1 << CS10; // 1024분주
	
	//----- EINT4 설정 -----
	// P287 그림12-6
	EIMSK |= 1 << INT4; //외부 INT4(ECHO핀) 설정
}

void make_trigger(void) {
	TRIG_PORT &= ~(1 << TRIG_PIN); // low로 만든다.
	_delay_us(1);
	TRIG_PORT |= (1 << TRIG_PIN); // high로 만든다.
	_delay_us(15);	//규격에는 10us인데 redunacy로 15us 
	TRIG_PORT &= ~(1 << TRIG_PIN);
}

void ultrasonic_processing(void) {
		if (flag == 1){
			flag = 0;
			printf("%s", scm);
			make_trigger();
		}
}