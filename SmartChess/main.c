/* Smart Chess 1.0
 *	PB1-PB3 Output to the reed net
 *  PB0, PD7-PD5, PB7-PB6, PD4-PD3 Input from the reed net
 * IDEAS:
 *	Light sensor that lights up the board when there is not enough light to see
 *	Time control via timers + display?
 *	Display of the battery of the board
 */
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "piece.h"
#include "board.h"
#include "usart.h"


//Pre: "output" is an array of at least 8 chars in a known state.
//Post: The row read data is saved to the array
void readRow(unsigned char* output) {
	output[0] = (uint8_t)(PORTB>>PB0)&1;
	output[1] = (uint8_t)(PORTD>>PD7)&1;
	output[2] = (uint8_t)(PORTD>>PD6)&1;
	output[3] = (uint8_t)(PORTD>>PD5)&1;
	output[4] = (uint8_t)(PORTB>>PB7)&1;
	output[5] = (uint8_t)(PORTB>>PB6)&1;
	output[6] = (uint8_t)(PORTD>>PD4)&1;
	output[7] = (uint8_t)(PORTD>>PD3)&1;
}

//Pre: "output" is an array of 144 chars in a known state.
//Post: The board is read and stored in "output".
void readBoard(unsigned char* output) {
	int i=26; //for offset
	for (int j = 0; j<8; j++) {
		PORTB |= (j<<PB1);
		readRow(&output[i]);
		PORTB &= ~(j<<PB1);
		i+=MOVE_N;	
	}
}


int main(void)
{
	// PB1-PB3 Output to the reed net
	// PB0, PD7-PD5, PB7-PB6, PD4-PD3 Input from the reed net
	// PD0-PD1 output to serial
	DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB3);
	USART_init(UBRR);
	
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