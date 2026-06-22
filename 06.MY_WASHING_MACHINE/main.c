/*
 * 05.ULTRASONIC.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#define RUN 1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수
#include <stdio.h>


volatile uint32_t ms_count = 0; // volatile 최적화 방지
volatile uint32_t wt_count = 0;
uint32_t washing_time = 0;
uint32_t rinse_time = 0;
uint32_t spin_time = 0;
volatile int motor_direction = 1;
volatile int run = 0;

ISR(TIMER0_OVF_vect) {
	TCNT0 = 6;
	ms_count++; // 1ms count
	
	if (run == RUN)
		wt_count++;
}

void init_timer0(void);

extern void init_led(void);
extern int led_main(void);
extern void init_ultrasonic(void);
extern void led_flower_off(void);
extern void ultrasonic_processing(void);
extern void init_uart0(void);
extern void UART0_transmit(void);
extern int ultrasonic_distance;
extern void led_distance_change_on(void);
extern void init_fnd(void);
extern void init_timer3_pwm(void);
extern void init_button(void);
extern void init_motor_driver(void);
extern void dcmotor_pwm_control_main(void);
extern void washing_machine_main(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
int main(void)
{

	init_led();
	init_fnd();
	init_button();
	init_timer0();
	init_uart0();
	init_motor_driver();
	init_timer3_pwm();
	
	stdout = &OUTPUT;
	sei();
    while (1) {
		washing_machine_main();
    }
}

void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}
