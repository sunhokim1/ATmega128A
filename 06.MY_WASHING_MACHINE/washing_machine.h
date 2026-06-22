/*
 * washing_machine.h
 *
 * Created: 2026-06-18 오후 4:13:03
 *  Author: kccistc
 */ 


#define F_CPU 16000000UL  //16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>
#include <stdio.h>

#define FORWARD 0
#define REVERSE 1

#define READY_MODE 0
#define WASHING_MODE 1
#define RINSE_MODE 2
#define SPIN_MODE 3

#define RUN 1
#define STOP 0