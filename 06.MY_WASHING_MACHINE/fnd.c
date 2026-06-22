/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:46
 *  Author: kccistc
 */ 

#include "fnd.h"
#include "washing_machine.h"

void init_fnd(void);
void fnd_display(int mode);
void fnd_set_time_display(void);

extern volatile uint32_t ms_count; //ms를 재는 count uint32_t : unsigned int
extern uint32_t washing_time;
extern uint32_t rinse_time;
extern uint32_t spin_time;
extern int current_setting_mode;
extern int current_run_mode;

void init_fnd(void) {
	FND_DATA_DDR = 0xff; //출력모드로 설정한다.
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4;
	FND_DATA_PORT = (uint8_t)~0xff;
}

void fnd_set_time_display(void) {
	uint8_t fnd_font[] = { (uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};
		
	static int digit_select = 0; //자리 수 선택
	if (current_setting_mode == READY_MODE){
		switch(digit_select) {
			case 0 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}else if (current_setting_mode == WASHING_MODE){
		switch(digit_select) {
			case 0 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[washing_time / 60 % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[washing_time / 60 / 10 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[1];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}else if (current_setting_mode == RINSE_MODE){
		switch(digit_select) {
			case 0 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[rinse_time / 60 % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[rinse_time / 60 / 10 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[2];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}else if (current_setting_mode == SPIN_MODE){
		switch(digit_select) {
			case 0 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[spin_time / 60 % 10];
			FND_DIGIT_PORT = (uint8_t)~0x80;
			break;

			case 1 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[spin_time / 60 / 10 % 6];
			FND_DIGIT_PORT = (uint8_t)~0x40;
			break;

			case 2 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[0];
			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;
			FND_DATA_PORT = fnd_font[3];
			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}
	
	digit_select = (digit_select + 1) % 4;
}
 

void fnd_display(int motor_direction) {
						// 0	 1	   2	  3	   4	 5	   6	 7	   8	  9		.
	uint8_t fnd_font[] = { (uint8_t)~0xc0, (uint8_t)~0xf9, (uint8_t)~0xa4, (uint8_t)~0xb0, (uint8_t)~0x99, (uint8_t)~0x92, (uint8_t)~0x82, (uint8_t)~0xd8, (uint8_t)~0x80, (uint8_t)~0x98, (uint8_t)~0x7f};
	static uint8_t circle = 0;
	static int digit_select = 0;
	static uint32_t prev_ms = 0;

	if (ms_count - prev_ms >= 200) {
		prev_ms = ms_count;

		if (motor_direction == FORWARD) {
			circle = (circle + 1) % 8;
		}
		else if (motor_direction == REVERSE) {
			circle = (circle + 7) % 8;
		}
	}
	
	
	if (motor_direction == FORWARD) {
		if (current_run_mode == WASHING_MODE) {
			switch(digit_select) {
				
				case 0 :
					if (washing_time < 60) {
						FND_DIGIT_PORT = 0xff;
						FND_DATA_PORT = fnd_font[washing_time % 10];
						FND_DIGIT_PORT = (uint8_t)~0x80;
						break;
					}else {
						FND_DIGIT_PORT = 0xff;
						FND_DATA_PORT = fnd_font[washing_time / 60 % 10];
						FND_DIGIT_PORT = (uint8_t)~0x80;
						break;
					}
				case 1 :
					if (washing_time < 60){
						FND_DIGIT_PORT = 0xff;
						FND_DATA_PORT = fnd_font[washing_time / 10 % 6];
						FND_DIGIT_PORT = (uint8_t)~0x40;
						break;
					}else {
						FND_DIGIT_PORT = 0xff;
						FND_DATA_PORT = fnd_font[washing_time / 600 % 6];
						FND_DIGIT_PORT = (uint8_t)~0x40;
						break;
					}
				case 2 :
				FND_DIGIT_PORT = 0xff;

				if (circle == 3) FND_DATA_PORT = 0x08;
				else if (circle == 4) FND_DATA_PORT = 0x04;
				else if (circle == 5) FND_DATA_PORT = 0x02;
				else if (circle == 6) FND_DATA_PORT = 0x01;
				else FND_DATA_PORT = 0x00;

				FND_DIGIT_PORT = (uint8_t)~0x20;
				break;
				
				case 3 :
				FND_DIGIT_PORT = 0xff;

				if (circle == 0) FND_DATA_PORT = 0x20;
				else if (circle == 1) FND_DATA_PORT = 0x10;
				else if (circle == 2) FND_DATA_PORT = 0x08;
				else if (circle == 7) FND_DATA_PORT = 0x01;
				else FND_DATA_PORT = 0x00;

				FND_DIGIT_PORT = (uint8_t)~0x10;
				break;
			}
		}else if (current_run_mode == RINSE_MODE) {
			switch(digit_select) {
				case 0 :
				if (rinse_time < 60) {
					FND_DIGIT_PORT = 0xff;
					FND_DATA_PORT = fnd_font[rinse_time % 10];
					FND_DIGIT_PORT = (uint8_t)~0x80;
					break;
				}else {
					FND_DIGIT_PORT = 0xff;
					FND_DATA_PORT = fnd_font[rinse_time / 60 % 10];
					FND_DIGIT_PORT = (uint8_t)~0x80;
					break;
				}
				case 1 :
				if (rinse_time < 60){
					FND_DIGIT_PORT = 0xff;
					FND_DATA_PORT = fnd_font[rinse_time / 10 % 6];
					FND_DIGIT_PORT = (uint8_t)~0x40;
					break;
				}else {
					FND_DIGIT_PORT = 0xff;
					FND_DATA_PORT = fnd_font[rinse_time / 600 % 6];
					FND_DIGIT_PORT = (uint8_t)~0x40;
					break;
				}
				case 2 :
				FND_DIGIT_PORT = 0xff;

				if (circle == 3) FND_DATA_PORT = 0x08;
				else if (circle == 4) FND_DATA_PORT = 0x04;
				else if (circle == 5) FND_DATA_PORT = 0x02;
				else if (circle == 6) FND_DATA_PORT = 0x01;
				else FND_DATA_PORT = 0x00;

				FND_DIGIT_PORT = (uint8_t)~0x20;
				break;

				case 3 :
				FND_DIGIT_PORT = 0xff;

				if (circle == 0) FND_DATA_PORT = 0x20;
				else if (circle == 1) FND_DATA_PORT = 0x10;
				else if (circle == 2) FND_DATA_PORT = 0x08;
				else if (circle == 7) FND_DATA_PORT = 0x01;
				else FND_DATA_PORT = 0x00;

				FND_DIGIT_PORT = (uint8_t)~0x10;
				break;
			}
		}else if (current_run_mode == SPIN_MODE) {
		switch(digit_select) {
			case 0 :
			if (spin_time < 60) {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 60 % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
			}
			case 1 :
			if (spin_time < 60){
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 10 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 600 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
			}
			case 2 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 3) FND_DATA_PORT = 0x08;
			else if (circle == 4) FND_DATA_PORT = 0x04;
			else if (circle == 5) FND_DATA_PORT = 0x02;
			else if (circle == 6) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 0) FND_DATA_PORT = 0x20;
			else if (circle == 1) FND_DATA_PORT = 0x10;
			else if (circle == 2) FND_DATA_PORT = 0x08;
			else if (circle == 7) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}
	}else if (motor_direction == REVERSE) {
	if (current_run_mode == WASHING_MODE) {
		switch(digit_select) {
			
			case 0 :
			if (washing_time < 60) {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[washing_time % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[washing_time / 60 % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
			}
			case 1 :
			if (washing_time < 60){
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[washing_time / 10 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[washing_time / 600 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
			}
			case 2 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 3) FND_DATA_PORT = 0x08;
			else if (circle == 4) FND_DATA_PORT = 0x04;
			else if (circle == 5) FND_DATA_PORT = 0x02;
			else if (circle == 6) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;
			
			case 3 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 0) FND_DATA_PORT = 0x20;
			else if (circle == 1) FND_DATA_PORT = 0x10;
			else if (circle == 2) FND_DATA_PORT = 0x08;
			else if (circle == 7) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
		}else if (current_run_mode == RINSE_MODE) {
		switch(digit_select) {
			case 0 :
			if (rinse_time < 60) {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[rinse_time % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[rinse_time / 60 % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
			}
			case 1 :
			if (rinse_time < 60){
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[rinse_time / 10 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[rinse_time / 600 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
			}
			case 2 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 3) FND_DATA_PORT = 0x08;
			else if (circle == 4) FND_DATA_PORT = 0x04;
			else if (circle == 5) FND_DATA_PORT = 0x02;
			else if (circle == 6) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 0) FND_DATA_PORT = 0x20;
			else if (circle == 1) FND_DATA_PORT = 0x10;
			else if (circle == 2) FND_DATA_PORT = 0x08;
			else if (circle == 7) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
		}else if (current_run_mode == SPIN_MODE) {
		switch(digit_select) {
			case 0 :
			if (spin_time < 60) {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 60 % 10];
				FND_DIGIT_PORT = (uint8_t)~0x80;
				break;
			}
			case 1 :
			if (spin_time < 60){
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 10 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
				}else {
				FND_DIGIT_PORT = 0xff;
				FND_DATA_PORT = fnd_font[spin_time / 600 % 6];
				FND_DIGIT_PORT = (uint8_t)~0x40;
				break;
			}
			case 2 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 3) FND_DATA_PORT = 0x08;
			else if (circle == 4) FND_DATA_PORT = 0x04;
			else if (circle == 5) FND_DATA_PORT = 0x02;
			else if (circle == 6) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x20;
			break;

			case 3 :
			FND_DIGIT_PORT = 0xff;

			if (circle == 0) FND_DATA_PORT = 0x20;
			else if (circle == 1) FND_DATA_PORT = 0x10;
			else if (circle == 2) FND_DATA_PORT = 0x08;
			else if (circle == 7) FND_DATA_PORT = 0x01;
			else FND_DATA_PORT = 0x00;

			FND_DIGIT_PORT = (uint8_t)~0x10;
			break;
		}
	}
	}
	digit_select = (digit_select + 1) % 4;
}

