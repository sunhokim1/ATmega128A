/*
 * 09.DHT11.c
 *
 * Created: 2026-06-26 오전 9:27:46
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> //인터럽트 관련 함수
#include <stdio.h>

extern void init_uart0(void);
extern void UART0_transmit(void);
extern void dht11_main(void);

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{


	init_uart0();
	stdout = &OUTPUT;

    while (1) {
		dht11_main();
		_delay_ms(1500);
    }
}
