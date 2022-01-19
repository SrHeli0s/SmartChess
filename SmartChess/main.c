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

const uint8_t MASKS[8] = {(1>>PINB1), (1>>PB2), (1>>PC0), (1>>PC1), (1>>PC2), (1>>PC3), (1>>PC4), (1>>PC5)};

//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char output[]) {
	unsigned char buffer[20] = "";
	itoa((PINB & MASKS[0]),buffer,2);
	USART_transmit_str(buffer);
	if ((PINB & MASKS[0]) == 0) {
		output[0] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[0] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINB & MASKS[1]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINB & MASKS[1]) == 0) {
		output[1] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[1] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[2]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[2]) == 0) {
		output[2] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[2] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[3]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[3]) == 0) {
		output[3] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[3] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[4]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[4]) == 0) {
		output[4] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[4] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[5]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[5]) == 0) {
		output[5] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[5] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[6]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[6]) == 0) {
		output[6] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[6] = 1;
		//USART_block_transmit('X');
	}
	
	itoa((PINC & MASKS[7]),buffer,2);
	USART_transmit_str(buffer);
	
	if ((PINC & MASKS[7]) == 0) {
		output[7] = 0;
		//USART_block_transmit('0');
	}
	else {
		output[7] = 1;
		//USART_block_transmit('X');
	}

	USART_block_transmit('\n');
	
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
	
	USART_block_transmit('\n');
	
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
	 *	PD3-7 PB6-7 PB0 Output to the reed net
	 *  PC0-5 PB1-2 Input from the reed net
	 *  PD2 External interrupt
	 */
	DDRD |= (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
	DDRB |= (1<<PB6) | (1<<PB7) | (1<<PB0);
	DDRB |= (1<<PB5);
	PORTB |= (1<<PB5);
	USART_init(UBRR);
	prepareGame();
	EIMSK |= (1<<INT0); //Enable external interruptions on pin INT0 (=PD2)
	EICRA |= (1<<ISC11); //Falling edge on INT0 generates an interrupt request.
	sei();
	if (DEBUG_MODE == 1) {
		_delay_ms(2000);
		USART_transmit_str("BEGIN DEBUG");
		PORTB &= ~(1<<PB5);
		while(1) {
			//USART_transmit_board(detected_position);
			readBoard(detected_position);
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
			readBoard(detected_position);
			if(compareBoards(detected_position,actual_position) != 0) {
				if(compareBoards(actual_position,middle_position) == 0) {
					copyBoards(detected_position,middle_position);
				}
			}
		}
		
		
	}
}