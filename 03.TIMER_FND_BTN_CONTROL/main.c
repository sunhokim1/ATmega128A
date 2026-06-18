/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:08
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#define RUN 1
#define STOP 0
#define STOPWATCH 2
#include <avr/io.h>
#include "button.h"
#include "fnd.h"
#include <avr/interrupt.h> //인터럽트 관련 함수

extern void fnd_display(int mode);

volatile uint32_t sec_count = 0;
volatile uint8_t dot_display;
volatile uint32_t ms_count = 0;
volatile uint32_t wt_count = 0;
volatile int run = STOP;
volatile int select_mode = 0;

ISR(TIMER2_OVF_vect)
{
	TCNT2 = 194;

	ms_count++;   // 버튼 디바운싱용, 항상 증가

	if (select_mode != STOPWATCH || run == RUN) {
		wt_count++;
	}
}


extern void init_button(void);
extern void init_fnd(void);
extern int get_button(int button_num, int button_pin);

void init_timer2(void);
void play_clock(int mode, int run);

int main(void)
{


	init_fnd();
	init_button();
	init_timer2();

	while (1)
	{
		play_clock(select_mode, run);
		if (get_button(BUTTON0, BUTTON0PIN)) {
				wt_count = 0;
				sec_count = 0;
				select_mode++;
			if (select_mode == 3)
				select_mode = 0;

			if (select_mode == STOPWATCH)
				run = STOP;
			else
				run = RUN;
		}

		
		if (get_button(BUTTON1, BUTTON1PIN) && select_mode == STOPWATCH) {
			run = !run;
		}

		if (get_button(BUTTON2, BUTTON2PIN)) {
			ms_count = 0;
			sec_count = 0;
			wt_count = 0;
			run = STOP;
		}
	}
}

void play_clock(int mode, int run)
{
	fnd_display(mode);
	if (wt_count >= 1000) {
		wt_count = 0;
		sec_count++;
		dot_display = !dot_display;
	}
	
}

void init_timer2(void)
{
	TCNT2 = 194;

	TCCR2 = 0x00;
	TCCR2 = (1 << CS22);

	TIMSK |= (1 << TOIE2);
	sei();
}