/* Smart Chess 1.0
 *	PD5-7 Output to the reed net
 *  PC0-5 PB1-2 Input from the reed net
 *  PD2 External interrupt
 * IDEAS:
 *	Light sensor that lights up the board when there is not enough light to see
 *	Time control via timers + display?
 *	Display of the battery of the board
 */

#define SHOWCASE_MODE 1
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "piece.h"
#include "board.h"
#include "usart.h"


//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char* output) {
	output[0] = (uint8_t)(PORTB>>PB1)&1;
	output[1] = (uint8_t)(PORTB>>PB2)&1;
	output[2] = (uint8_t)(PORTC>>PC0)&1;
	output[3] = (uint8_t)(PORTC>>PC1)&1;
	output[4] = (uint8_t)(PORTC>>PC2)&1;
	output[5] = (uint8_t)(PORTC>>PC3)&1;
	output[6] = (uint8_t)(PORTC>>PC4)&1;
	output[7] = (uint8_t)(PORTC>>PC5)&1;
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard(unsigned char* output) {
	int i=26; //for offset
	for (int j = 0; j<8; j++) {
		PORTD |= (j<<PD5);
		readRow(&output[i]);
		PORTD &= ~(j<<PD5);
		i+=MOVE_N;	
	}
}

ISR(INT0_vect) { //The button has been pressed
	char move[] = "";
	//TODO: What happens if this fails?
	translateMove(detected_position,middle_position,move);
	
	if (makeMove(detected_position,middle_position) == 0) { //Legal move
		USART_transmit_str(move);
	}
	else { //Ilegal move
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
	USART_init(UBRR);
	EIMSK |= (1<<INT0); //Enable external interruptions on pin INT0 (=PD2)
	EICRA |= (1<<ISC00); //Any logical change on INT0 generates an interrupt request.
	sei();
	
	if (SHOWCASE_MODE == 1) {
		while(1) {
			readBoard(detected_position);
			USART_transmit_board(detected_position);
			_delay_ms(1000);
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