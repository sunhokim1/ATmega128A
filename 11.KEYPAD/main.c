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
#include "queue.h"

extern void init_uart0(void);
extern void UART0_transmit(void);
extern void init_keypad(void);
extern uint8_t keypad_scan(void);
void init_timer0(void);
char buff[20];
extern int calculator(char* buff);

volatile uint32_t keypad_counter = 0;
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

ISR(TIMER0_OVF_vect) {
	volatile uint8_t keydata = 0;
	TCNT0 = 6;
	if (++keypad_counter >= 60) {
		keypad_counter=0;
		if (keydata = keypad_scan()) { // keypad를 check해서 눌려 진 것이 있으면
			insert_queue(keydata);
		}	// circular queue에 저장한다.
	}
}

int main(void) {
	uint8_t key_value;
	init_uart0();
	init_timer0();
	init_keypad();
	
	stdout = &OUTPUT;
	sei(); // 전역(대문) interrupt 허용

	int i = 0;
	while(1) {
		if (queue_empty() != TRUE) {
			key_value = read_queue();
			printf("key_value: %c\n", key_value);
			if (key_value == '=') {
				buff[i] = key_value;
				buff[i+1] = '\0';
				printf("%s\n", buff);
				if (calculator(buff) == -999)
					printf("wrong num \n");
				else{
					printf("cal : %s %d\n", buff, calculator(buff));
				}
				i = 0;
				buff[i] = '\0';
			}else if ( (buff[i - 1] == '-' || buff[i - 1] == '+' || buff[i - 1] == '/' || buff[i - 1] == '*') &&
			(key_value == '-' || key_value == '+' || key_value == '/' || key_value == '*')) {
				i--;
				buff[i] = key_value;
				buff[i+1] = '\0';
				printf("cal : %s\n", buff);
				i++;
			}else {
				buff[i] = key_value;
				buff[i+1] = '\0';
				printf("cal : %s\n", buff);
				i++;
			}
		}
		
	} 
}

void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //6분주 
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}
