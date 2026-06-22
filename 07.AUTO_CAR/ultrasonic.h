/*
 * ultrasonic.h
 *
 * Created: 2026-06-17 오후 1:32:30
 *  Author: kccistc
 */ 


#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>
#include <stdio.h>

#define TRIG_DDR	DDRA
#define TRIG_PORT	PORTA
#define TRIG_PIN_L	0
#define TRIG_PIN_C	1
#define TRIG_PIN_R	2

#define ECHO_DDR	DDRE
#define ECHO_PORT	PINE	// external Int 4
#define ECHO_PIN_L	4
#define ECHO_PIN_C	5
#define ECHO_PIN_R	6
