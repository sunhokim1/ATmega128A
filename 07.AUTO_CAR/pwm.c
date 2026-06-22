/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:25:40
 *  Author: kccistc
 */ 

#include "pwm.h"
#include "button.h"

extern int get_button(int button_num, int button_pin);


/*
	PWM 출력 신호
	============
	PB5 : OC1A : 왼쪽 바퀴
	PB6 : OC1B : 오른쪽 바퀴
	BTN0 : auto/manual mode 전환
	
	방향 설정
	========
	1. 왼쪽 바퀴
		PORTF0 -- IN1 (DC motor driver)
		PORTF1 -- IN2
		
	2. 오른쪽 바퀴
		PORTF2 -- IN3 (DC motor driver)
		PORTF3 -- IN4
		
		IN1/IN3	 IN2/IN4
		=======	 =======
		   0        1    : 역회전
		   1        0    : 정회전
		   1        1    : stop
*/

#define MOTOR_PWM_DDR DDRB
#define MOTOR_LEFT_PORT_DDR 5 // OC1A
#define MOTOR_RIGHT_PORT_DDR 6 // OC1B
#define MOTOR_DIRECTION_PORT PORTF
#define MOTOR_DIRECTION_PORT_DDR DDRF
void init_motor_driver(void);
void init_timer1_pwm(void);
void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
void stop(void);


void init_timer1_pwm(void) {
		
	//---- 분주비 설정 ----
	// 분주비 64
	// 16000000Hz / 64 --> 250000Hz (250kHz)
	// T = 1/f 1/250000Hz --> 0.000004sec --> 4us
	// 250000Hz에서 256개 펄스를 count하면 소요 시간 : 1.02ms
	//              127개                          : 0.5ms
	//				0x03ff(1023)	---> 4ms
	//TCNT3 : 0~255(0x00ff) 까지 count한 후 0으로 다시 돌아간다.
	TCCR1B |= 1 << CS11 | 1 << CS10; // 분주비 64
	// OCR3C : 50 인 경우 Duty (HIGH)가 몇 % 인가?
	// Duty Cycle : (OCR3C / TOP) * 100 = 50 / 255 * 100 = 19.61%
	
	// 모드 14 | 16bit 고속 PWM 모드 사용하겠다. timer1 ( P348 표 15-6)
	TCCR1A |= 1 << WGM11;	// TOP --> ICR1으로 설정한다.
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	// 비반전 모드 top : ICR1 비교일치 값 (PWM) 지정 OCR1A OCR1B P350 표 15-7)
	// 비교 일치 발생 시 OCR1, OCR1B의 출력핀은 LOW로 바뀌고 BOTTON에서 HIGH로 바뀐다.
	TCCR1A |= 1 << COM1A1;
	TCCR1A |= 1 << COM1B1;
	
	ICR1 = 0x3ff;	//1023 * 4us ==> 4ms TOP 
}


/*
	PE5 : PWM control
	PE6 : IN1 방향 설정
	PE7 : IN2
*/
void init_motor_driver(void) {
	// 1. 출력 모드로 설정
	MOTOR_PWM_DDR &= ~(1 << 5 | 1 << 6); //0으로 초기화 하고 시작
	MOTOR_PWM_DDR |= 1 << MOTOR_LEFT_PORT_DDR | 1 << MOTOR_RIGHT_PORT_DDR;
	MOTOR_DIRECTION_PORT_DDR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // 0으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 ;	//출력모드로 설정
	
	// 2. 모터를 전진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 1 | 1 << 3; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
											 //					0	1	0	1
}

void backward(int speed) {
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
	
	OCR1A=OCR1B=speed;
	
}

void forward(int speed) {
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 1 | 1 << 3; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
	
	OCR1A=OCR1B=speed;
}

void turn_right(int speed) {
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 1 | 1 << 3; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
	
	OCR1A=speed;
	OCR1B=0;
}

void turn_left(int speed) {
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 1 | 1 << 3; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
	
	OCR1A=0;
	OCR1B=speed;
}

void stop(void) {
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3); // ~0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3; // 모터를 전진 모드로 IN4IN3IN2IN1 1010
	
	OCR1A=0;
	OCR1B=0;
}