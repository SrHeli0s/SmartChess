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
#define BAUDRATE 9600
#define UBRR (F_CPU/(BAUDRATE<<4))-1

#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "chess.h"




//Pre: The USART is in a unknown/bad state
//Post: The USART is initialized and ready to use
void USART_init(unsigned int baud) {
	DDRD |= (1<<PD1);
	UBRR0H = 0;
	UBRR0L = 51;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);	//Enable receiver and transmitter
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);	//Set frame format: 8 data, 1 stop bit
}

//Pre: "data" contains a character to send using USART
//Post: The function sends the character when possible
void USART_block_transmit(unsigned char data) {
	while (!(UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
	UDR0 = data; // Put data into buffer and send the data 
}

//Pre: "data" contains a character to send using USART
//Post: The function returns 0 if sent the character, -1 if is unable to do it.
int8_t USART_noblock_transmit(unsigned char data) {
	if (!(UCSR0A & (1<<UDRE0))) { return -1; }; // Wait for empty transmit buffer
	UDR0 = data; // Put data into buffer and send the data
	return 0;
}

//Pre: "str" is a pointer to a null terminated string
//Post: The string is sent using USART
void USART_transmit_str(const char* str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		USART_block_transmit(str[j]);
		j++;
	}
}

unsigned char USART_block_recieve() {
		while ((UCSR0A & (1 << RXC0)) == 0) {};	// Wait till data is received
		return(UDR0);		// Return the byte
}

unsigned char USART_noblock_recieve() {
	return(UDR0);		// Return the byte
}

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
	
		
	}
}