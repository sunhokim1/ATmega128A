/*
 * main.c
 *
 * Created: 2026-06-22 오후 8:20:39
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define TRIG_DDR    DDRA
#define TRIG_PORT   PORTA

#define TRIG_L      PA0
#define TRIG_C      PA1
#define TRIG_R      PA2

#define ECHO_DDR    DDRE
#define ECHO_PIN    PINE

void UART0_transmit(uint8_t data);
void init_uart0(void);
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint16_t start4 = 0;
volatile uint16_t start5 = 0;
volatile uint16_t start6 = 0;

volatile uint16_t dist4 = 0;
volatile uint16_t dist5 = 0;
volatile uint16_t dist6 = 0;

volatile uint8_t got4 = 0;
volatile uint8_t got5 = 0;
volatile uint8_t got6 = 0;


void init_ultrasonic_test(void);
void make_trigger(uint8_t pin);

void UART0_transmit(uint8_t data)
{
	while (!(UCSR0A & (1 << UDRE0)))
	;
	UDR0 = data;
}

void init_uart0(void)
{
	UBRR0H = 0x00;
	UBRR0L = 207;              // 16MHz, U2X=1, 9600bps
	UCSR0A |= (1 << U2X0);
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void make_trigger(uint8_t pin)
{
	TRIG_PORT &= ~(1 << pin);
	_delay_us(2);

	TRIG_PORT |= (1 << pin);
	_delay_us(15);

	TRIG_PORT &= ~(1 << pin);
}

ISR(INT4_vect)
{
	if (ECHO_PIN & (1 << PE4)) {
		start4 = TCNT3;
		} else {
		uint16_t diff = TCNT3 - start4;
		dist4 = (diff * 64UL) / 58;
		got4 = 1;
	}
}

ISR(INT5_vect)
{
	if (ECHO_PIN & (1 << PE5)) {
		start5 = TCNT3;
		} else {
		uint16_t diff = TCNT3 - start5;
		dist5 = (diff * 64UL) / 58;
		got5 = 1;
	}
}

ISR(INT6_vect)
{
	if (ECHO_PIN & (1 << PE6)) {
		start6 = TCNT3;
		} else {
		uint16_t diff = TCNT3 - start6;
		dist6 = (diff * 64UL) / 58;
		got6 = 1;
	}
}

void init_ultrasonic_test(void)
{
	TRIG_DDR |= (1 << TRIG_L) | (1 << TRIG_C) | (1 << TRIG_R);
	TRIG_PORT &= ~((1 << TRIG_L) | (1 << TRIG_C) | (1 << TRIG_R));

	ECHO_DDR &= ~((1 << PE4) | (1 << PE5) | (1 << PE6));
	PORTE &= ~((1 << PE4) | (1 << PE5) | (1 << PE6)); // pull-up off

	EICRB &= ~((1 << ISC41) | (1 << ISC40) |
	(1 << ISC51) | (1 << ISC50) |
	(1 << ISC61) | (1 << ISC60));

	EICRB |= (1 << ISC40) | (1 << ISC50) | (1 << ISC60); // any change

	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3 = 0;
	TCCR3B |= (1 << CS32) | (1 << CS30); // 1024 prescaler

	EIFR |= (1 << INTF4) | (1 << INTF5) | (1 << INTF6);
	EIMSK |= (1 << INT4) | (1 << INT5) | (1 << INT6);
}
	int main(void)
	{
		init_uart0();
		stdout = &OUTPUT;

		printf("ULTRASONIC TEST START\r\n");

		init_ultrasonic_test();
		sei();

		while (1)
		{
			got4 = 0;
			printf("\r\n[L] TRIG PA0 / ECHO PE4\r\n");
			make_trigger(TRIG_L);
			_delay_ms(80);

			if (got4)
			printf("INT4 OK, distance=%u cm\r\n", dist4);
			else
			printf("INT4 FAIL\r\n");

			got5 = 0;
			printf("[C] TRIG PA1 / ECHO PE5\r\n");
			make_trigger(TRIG_C);
			_delay_ms(80);

			if (got5)
			printf("INT5 OK, distance=%u cm\r\n", dist5);
			else
			printf("INT5 FAIL\r\n");

			got6 = 0;
			printf("[R] TRIG PA2 / ECHO PE6\r\n");
			make_trigger(TRIG_R);
			_delay_ms(80);

			if (got6)
			printf("INT6 OK, distance=%u cm\r\n", dist6);
			else
			printf("INT6 FAIL\r\n");

			printf("PINE=%02X\r\n", PINE);

			_delay_ms(1000);
		}
	}
