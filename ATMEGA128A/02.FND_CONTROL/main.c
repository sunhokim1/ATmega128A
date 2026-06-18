/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:08
 * Author : kccistc
 */ 

#include <avr/io.h>

extern int fnd_main(void);
extern void init_fnd(void);
extern void init_button(void);


int main(void)
{
	init_button();
	init_fnd();
    
	while (1) {
		
    }
}

