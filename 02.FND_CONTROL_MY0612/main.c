/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:08
 * Author : kccistc
 */ 


#define RUN 1
#define STOP 0
#include <avr/io.h>
#include "button.h"
#include "fnd.h"


extern void fnd_display(int mode);
extern void init_button(void);
extern void init_fnd(void);
extern int get_button(int button_num, int button_pin);

void play_clock(int mode, int run);
	
int main(void)
{
	init_button();
	init_fnd();
	
	int select_mode = 0;
	int run = STOP;
	while (1) 
    {
		play_clock(select_mode, run);
		
		if (get_button(BUTTON0, BUTTON0PIN)) {
			ms_count = 0;
			sec_count = 0;
			run = STOP;
			select_mode++;
		}
		if (select_mode == 3)
			select_mode = 0;
		if (get_button(BUTTON1, BUTTON1PIN)) {
			run = !run;
		}
		if (get_button(BUTTON2, BUTTON2PIN)) {
			ms_count = 0;
			sec_count = 0;
			run = STOP;
		}
    }
	
	return 0;
}

void play_clock(int mode, int run) {
	fnd_display(mode);
	_delay_ms(1);
	if (!(mode == 2 && run == STOP))
		ms_count++;
	if (ms_count >= 1000) { // 1000ms --> 1sec
		ms_count = 0;
		sec_count++;
		dot_display = !dot_display;
	}
}