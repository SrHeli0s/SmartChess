/* Smart Chess 1.1
 *  PD3-7 PB6-7 PB0 Output to the reed net
 *  PC0-5 PB2-3 Input from the reed net
 *  PB1 Blinking LED
 *  PB4 Red LED
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
#include "piece.h"
#include "board.h"
#include "usart.h"


//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char output[]) {
	if (PINB & (1<<PINB2)) {
		output[0] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[0] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINB & (1<<PINB3)) {
		output[1] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[1] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC0)) {
		output[2] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[2] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC1)) {
		output[3] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[3] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC2)) {
		output[4] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[4] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC3)) {
		output[5] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[5] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC4)) {
		output[6] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[6] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}
	
	
	if (PINC & (1<<PINC5)) {
		output[7] = 0;
		if(DEBUG_MODE==1) { USART_block_transmit('0'); }
	}
	else {
		output[7] = 1;
		if(DEBUG_MODE==1) { USART_block_transmit('X'); }
	}

	if (DEBUG_MODE == 1) { USART_block_transmit('\n'); }
	
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard(unsigned char output[]) {
	int i=26; //for offset
	
	PORTD |= (1<<PD3);
	readRow(&output[i]);
	PORTD &= ~(1<<PD3);
	i+=MOVE_N;
	
	PORTD |= (1<<PD4);
	readRow(&output[i]);
	PORTD &= ~(1<<PD4);
	i+=MOVE_N;
	
	PORTB |= (1<<PB6);
	readRow(&output[i]);
	PORTB &= ~(1<<PB6);
	i+=MOVE_N;
	
	PORTB |= (1<<PB7);
	readRow(&output[i]);
	PORTB &= ~(1<<PB7);
	i+=MOVE_N;
	
	PORTD |= (1<<PD5);
	readRow(&output[i]);
	PORTD &= ~(1<<PD5);
	i+=MOVE_N;
	
	PORTD |= (1<<PD6);
	readRow(&output[i]);
	PORTD &= ~(1<<PD6);
	i+=MOVE_N;
	
	PORTD |= (1<<PD7);
	readRow(&output[i]);
	PORTD &= ~(1<<PD7);
	i+=MOVE_N;
	
	PORTB |= (1<<PB0);
	readRow(&output[i]);
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

ISR(INT0_vect) { //The button has been pressed
	if(DEBUG_MODE == 1) {
		USART_transmit_str("INTERRUPT\n");
		return;
	}
	char move[] = "";
	//NOTE: What happens if this fails?
	translateMove(detected_position,middle_position,move);
	
	if (makeMove(detected_position,middle_position) == 0) { //Legal move
		USART_transmit_str(move);
	}
	else { //Illegal move
		PORTB |= (1<<PB5);
		while(1) { //Wait until previous position is detected
			readBoard(detected_position);
			if (compareBoards(detected_position,actual_position) == 0) {
				PORTB &= ~(1<<PB5);
				break;
			}
		}
	}
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
		_delay_ms(2000);
		USART_transmit_str("BEGIN DEBUG");
		while(1) {
			//USART_transmit_board(detected_position);
			readBoard(detected_position);
		}
	}
	
	
	
	EIMSK |= (1<<INT0); //Enable external interruptions on pin INT0 (=PD2)
	EICRA |= (1<<ISC10); //Any logical change on INT0 generates an interrupt request.
	sei();
	
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
			readBoard(detected_position);
			if(compareBoards(detected_position,actual_position) != 0) {
				if(compareBoards(actual_position,middle_position) == 0) {
					copyBoards(detected_position,middle_position);
				}
			}
		}
		
		
	}
}