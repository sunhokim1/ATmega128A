/*
 * 08.PWM_SERVO_PIEZO_BUZZER.c
 *
 * Created: 2026-06-25 오전 9:46:21
 * Author : kccistc
 */ 



#if 1 // piezo buzzer

#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "button.h"

#define TUNE(a) ((F_CPU / (16 * a)) - 1);

extern void init_button(void);
extern int get_button(int button_num, int button_pin);

extern int lg_gajeon_Tune[];
extern const int lg_gajeon_Beats[];
extern void Music_Player(int *tone, int *Beats);
extern void init_speaker(void);
extern void Beep(int  repeat);
extern void Siren(int repeat);
extern void RRR(void);
extern void delay_ms(int ms);
extern const int Elise_Tune[];
extern const int Elise_Beats[];
extern int turkey_tune[];
extern const turkey_beats[];
void init_timer0(void);
volatile uint32_t ms_count = 0;

ISR(TIMER0_OVF_vect) {
	TCNT0 = 6;
	ms_count++;
}


// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.


void power_on_melody(void) {
	OCR3A = TUNE(1000);
	delay_ms(70);
	OCR3A = TUNE(2000);
	delay_ms(70);
	OCR3A = TUNE(3000);
	delay_ms(70);
	OCR3A = TUNE(4000);
	delay_ms(70);
	OCR3A = 0;
	delay_ms(3000);
}

void open_buzzer(void) {
	OCR3A = TUNE(261);
	delay_ms(70);
	OCR3A = TUNE(329);
	delay_ms(70);
	OCR3A = TUNE(392);
	delay_ms(70);
	OCR3A = TUNE(554);
	delay_ms(70);
	OCR3A = 0;
	delay_ms(3000);
}

int main(void)
{
	init_button();
	init_speaker();	
	init_timer0();
	
	sei();
	
	while(1)
	{
		if (get_button(BUTTON0, BUTTON0PIN)) {
			power_on_melody();
		}
		else if (get_button(BUTTON1, BUTTON1PIN)) {
			open_buzzer();
		}
		
		if (get_button(BUTTON2, BUTTON2PIN)) {
			Music_Player(turkey_tune, turkey_beats);
		}
		
/*
		ocr3a=1702;   // 삐~뽀 레 
		_delay_ms(1000);
		ocr3a=1431;   // 파  
		_delay_ms(1000);

*/
		
//		Beep(5);
//		 RRR();
 //		_delay_ms(3000);
 //		Siren(5);
 //		_delay_ms(3000);
//		Music_Player(Elise_Tune, Elise_Beats);		
	}
}

void init_timer0(void) {
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); // 초기화 (0분주)
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //64분주
	TIMSK |= 1 << TOIE0; // TIMER0 Overflow INT

}



#endif
#if 0 // servo motor
#include <avr/io.h>

extern int servo_motor_main(void);

int main(void)
{
	servo_motor_main();
    /* Replace with your application code */
    while (1) 
    {
    }
}

#endif