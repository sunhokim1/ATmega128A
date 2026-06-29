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
#include "ds1302.h"


extern void init_uart0(void);
extern void UART0_transmit(void);
extern void ds1302_main(void);
extern void pc_command_processing(char **func_name, t_ds1302 *ds1302);
extern void setrtc(char *str, t_ds1302 *ds1302);
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	init_uart0();
	stdout = &OUTPUT;
	sei();
	ds1302_main();
    while (1) {

    }
}
