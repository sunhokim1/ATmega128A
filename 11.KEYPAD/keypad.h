/*
 * keypad.h
 *
 * Created: 2026-06-29 오후 2:11:17
 *  Author: kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define KEYPAD_DDR DDRA
#define KEYPAD_PIN PINA
#define KEYPAD_PORT PORTA