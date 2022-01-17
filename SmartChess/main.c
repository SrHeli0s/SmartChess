/* Smart Chess 1.0
 *	PD5-7 Output to the reed net
 *  PC0-5 PB1-2 Input from the reed net
 *  PD2 External interrupt
 * IDEAS:
 *	Light sensor that lights up the board when there is not enough light to see
 *	Time control via timers + display?
 *	Display of the battery of the board
 */

#define DEBUG_MODE 1
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "piece.h"
#include "board.h"
#include "usart.h"

const uint8_t MASKS[8] = {(1>>PB1), (1>>PB2), (1>>PC0), (1>>PC1), (1>>PC2), (1>>PC3), (1>>PC4), (1>>PC5)};

//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char* output) {
	if ((PORTB & MASKS[0]) == 0) {
		output[0] = (uint8_t)0;
	}
	else {
		output[0] = (uint8_t)1;
	}
	if ((PORTB & MASKS[1]) == 0) {
		output[1] = (uint8_t)0;
	}
	else {
		output[1] = (uint8_t)1;
	}
	if ((PORTC & MASKS[2]) == 0) {
		output[2] = (uint8_t)0;
	}
	else {
		output[2] = (uint8_t)1;
	}
	
	if ((PORTC & MASKS[3]) == 0) {
		output[3] = (uint8_t)0;
	}
	else {
		output[3] = (uint8_t)1;
	}
	
	if ((PORTC & MASKS[4]) == 0) {
		output[4] = (uint8_t)0;
	}
	else {
		output[4] = (uint8_t)1;
	}
	
	if ((PORTC & MASKS[5]) == 0) {
		output[5] = (uint8_t)0;
	}
	else {
		output[5] = (uint8_t)1;
	}
	
	if ((PORTC & MASKS[6]) == 0) {
		output[6] = (uint8_t)0;
	}
	else {
		output[6] = (uint8_t)1;
	}
	
	if ((PORTC & MASKS[7]) == 0) {
		output[7] = (uint8_t)0;
	}
	else {
		output[7] = (uint8_t)1;
	}
	
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard(unsigned char* output) {
	int i=26; //for offset
	
	PORTD |= (1<<PD3);
	readRow(&output[i]);
	PORTD &= ~(1<<PD3);
	i+=MOVE_N;
	
	PORTD |= (1<<PD4);
	readRow(&output[i]);
	
	char buffer[10];
	itoa((uint8_t)PORTC,buffer,2);
	USART_transmit_str(buffer);
	USART_block_transmit('\n');
	
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
	
	//int i=26; //for offset
	//for (int j = 0; j<8; j++) {
		//PORTD |= (j<<PD5);
		//readRow(&output[i]);
		//PORTD &= ~(j<<PD5);
		//i+=MOVE_N;	
	//}
}

ISR(INT0_vect) { //The button has been pressed
	char move[] = "";
	//TODO: What happens if this fails?
	translateMove(detected_position,middle_position,move);
	
	if (makeMove(detected_position,middle_position) == 0) { //Legal move
		USART_transmit_str(move);
	}
	else { //Illegal move
		//TODO: set red led on
		while(1) { //Wait until previous position is detected
			readBoard(detected_position);
			if (compareBoards(detected_position,actual_position) == 0) {
				break;
			}
		}
	}
}

int main(void) {
	/*
	 *	PD5-7 Output to the reed net
	 *  PC0-5 PB1-2 Input from the reed net
	 *  PD2 External interrupt
	 */
	DDRD |= (1<<PD5) | (1<<PD6) | (1<<PD7);
	if (DEBUG_MODE == 1) {
		DDRD |= (1<<PD3) | (1<<PD4);
		DDRB |= (1<<PB6) | (1<<PB7) | (1<<PB0);
	}
	USART_init(UBRR);
	if (DEBUG_MODE == 1) {
		while(1) {
			readBoard(detected_position);
			USART_transmit_board(detected_position);
			_delay_ms(1000);
		}
	}
	
	EIMSK |= (1<<INT0); //Enable external interruptions on pin INT0 (=PD2)
	EICRA |= (1<<ISC00); //Any logical change on INT0 generates an interrupt request.
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