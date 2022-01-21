/* Smart Chess 1.1
 *  PD3-7 PB6-7 PB0 Output to the reed net
 *  PC0-5 PB2-3 Input from the reed net
 *  PB1 Blinking LED
 *  PB5 Red LED
 * IDEAS:
 *	Light sensor that lights up the board when there is not enough light to see
 *	Time control via timers + display?
 */

#define DEBUG_MODE 0
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
#define PRECSALER 1024
#define F_OUT 1 // output frequency
#define VALUE ((((F_CPU/2)/PRECSALER)/F_OUT)-1)

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "pieceShowcase.h"
#include "boardShowcase.h"
#include "usart.h"


//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(int i) {
	if (PINB & (1<<PINB2)) {
		detected_position[i+0] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+0] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINB & (1<<PINB3)) {
		detected_position[i+1] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+1] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC0)) {
		detected_position[i+2] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+2] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC1)) {
		detected_position[i+3] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+3] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC2)) {
		detected_position[i+4] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+4] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC3)) {
		detected_position[i+5] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+5] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC4)) {
		detected_position[i+6] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+6] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	
	
	if (PINC & (1<<PINC5)) {
		detected_position[i+7] = (uint8_t)1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	else {
		detected_position[i+7] = (uint8_t)0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}

	if (DEBUG_MODE == 1) { USART_block_transmit('\n'); }
	
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard() {
	int i=26; //for offset
	
	
	PORTD |= (1<<PD3);
	readRow(i);
	PORTD &= ~(1<<PD3);
	i+=MOVE_N;
	
	PORTD |= (1<<PD4);
	readRow(i);
	PORTD &= ~(1<<PD4);
	i+=MOVE_N;
	
	PORTB |= (1<<PB6);
	readRow(i);
	PORTB &= ~(1<<PB6);
	i+=MOVE_N;
	
	PORTB |= (1<<PB7);
	readRow(i);
	PORTB &= ~(1<<PB7);
	i+=MOVE_N;
	
	PORTD |= (1<<PD5);
	readRow(i);
	PORTD &= ~(1<<PD5);
	i+=MOVE_N;
	
	PORTD |= (1<<PD6);
	readRow(i);
	PORTD &= ~(1<<PD6);
	i+=MOVE_N;
	
	PORTD |= (1<<PD7);
	readRow(i);
	PORTD &= ~(1<<PD7);
	i+=MOVE_N;
	
	PORTB |= (1<<PB0);
	readRow(i);
	PORTB &= ~(1<<PB0);
	
	if (DEBUG_MODE == 1) { USART_block_transmit('\n'); }
	
	////Code for the DEMUX way:
	//int i=26; //for offset
	//for (int j = 0; j<8; j++) {
		//PORTD |= (j<<PD5);
		//readRow(&output[i]);
		//PORTD &= ~(j<<PD5);
		//i+=MOVE_N;	
	//}
}

int main(void) {
	/*
	 *  PD3-7 PB6-7 PB0 Output to the reed net
	 *  PC0-5 PB2-3 Input from the reed net
	 *  PB1 Blinking LED
	 *  PB5 Red LED
	 */
	DDRD |= (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB5) | (1<<PB6) | (1<<PB7);
	
	
	TCCR1A |= (1 << COM1A0); //Toggle on match
	TCCR1B |= (1 << WGM12); //CTC mode
	TCCR1B |= (1 << CS10) | (1 << CS12); //Prescaler to 1024
	OCR1A = VALUE; //Set max value
	
	USART_init(UBRR);
	prepareGame();
	if (DEBUG_MODE == 1) {
		PORTB |= (1<<PB5);
		_delay_ms(2000);
		USART_transmit_str("BEGIN DEBUG\n");
		USART_transmit_board(actual_position);
		PORTB &= ~(1<<PB5);
		while(1) {
			readBoard();
			USART_transmit_board(detected_position);
		}
	}
	
	char r = '#';
	while(1)
	{
		int validResponse = 0;
		while (!validResponse) {
			r = USART_block_recieve();
			if (r == 'S') {
				USART_block_transmit('s');
				validResponse = 1;	
			}
			else {
				USART_block_transmit('e');
			}
		}
		
		//Game loop
		while(1) {
			readBoard();
			//Check if new move:
			if (PIND & (1 << PIND2)) {
				if(compareBoards() != 0) {
					translateMove();
					if (makeMove() == 0) {
						USART_transmit_str(output);
					}
					else { //Illegal move
						PORTB |= (1<<PB5);
						while(1) { //Wait until previous position is detected
							readBoard(detected_position);
							if (compareBoards() == 0) {
								PORTB &= ~(1<<PB5);
								break;
							}
						}
					}
				}
			}
		}
		
		
	}
}
