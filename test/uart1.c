/*
 * uart1.c
 *
 * Created: 2026-06-22 오전 10:38:35
 *  Author: kccistc
 */ 

#include "uart1.h"

extern void UART0_transmit(uint8_t data);
void init_uart1(void);
void UART1_transmit(uint8_t bt_data);

volatile uint8_t bt_data;

// P278 표12-3
// PC로부터 1byte가 들어 오면 자동적으로 이곳으로 진입한다.
// 예) led_all_on\n 이면 11번 이곳으로 진입한다.
ISR(USART1_RX_vect) {
	
	bt_data = UDR1; // UDR1의 내용이 bt_data에 복사된다.
	UART0_transmit(bt_data);
}
/*
	1. 전송속도 : 9600bps
	2. start/stop 설정
	3. RX(수신) : interrupt로 설정
	TX(전송) : polling으로 처리한다.
	RX(수신) : interrupt로 처리한다.
*/
void init_uart1(void){
	// 1. 전송속도 : 9600bps
	UBRR1H = 0x00;
	UBRR1L = 207; // 9600bps 표 8-9
	UCSR1A |= 1 << U2X1; // 2배속 설정 (sampling8)
	// UART0를 송신, 수신이 다 가능하고 RX INT가 가능하도록 설정한다.
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
	
}

// UART0로 1byte를 전송 하는 함수
void UART1_transmit(uint8_t bt_data) {
	while (!(UCSR1A & 1 << UDRE1)) // data가 송신 중이면 송신이 끝날 때 까지 기다림
		;	// No operation
	UDR1 = bt_data; // HW 전송 register에 data를 송신한다.
}