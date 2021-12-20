/* Smart Chess 1.0
 *	PB1-PB5, PC0-PC2 Output to the reed net
 *  PB0, PD7-PD5, PB7-PB6, PD4-PD3 Input from the reed net
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
	
	//Read 1º row
	PORTB |= (1<<PB1);
	readRow(&output[i]);
	PORTB &= ~(1<<PB1);
	i+=MOVE_N;
	
	//Read 2º row
	PORTB |= (1<<PB2);
	readRow(&output[i]);
	PORTB &= ~(1<<PB2);
	i+=MOVE_N;
	
	//Read 3º row
	PORTB |= (1<<PB3);
	readRow(&output[i]);
	PORTB &= ~(1<<PB3);
	i+=MOVE_N;
	
	//Read 4º row
	PORTB |= (1<<PB4);
	readRow(&output[i]);
	PORTB &= ~(1<<PB4);
	i+=MOVE_N;
	
	//Read 5º row
	PORTB |= (1<<PB5);
	readRow(&output[i]);
	PORTB &= ~(1<<PB5);
	i+=MOVE_N;
	
	//Read 6º row
	PORTC |= (1<<PC0);
	readRow(&output[i]);
	PORTC &= ~(1<<PC0);
	i+=MOVE_N;
	
	//Read 7º row
	PORTC |= (1<<PC1);
	readRow(&output[i]);
	PORTC &= ~(1<<PC1);
	i+=MOVE_N;
	
	//Read 8º row
	PORTC |= (1<<PC2);
	readRow(&output[i]);
	PORTC &= ~(1<<PC2);
	
}



int main(void)
{
	// PB1-PB5, PC0-PC2 Output to the reed net
	// PB0, PD7-PD5, PB7-PB6, PD4-PD3 Input from the reed net
	// PD0-PD1 output to serial
	DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5);
	DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2);
	USART_init(UBRR);
	
	char r = '#';
	while(1)
	{
		r = USART_block_recieve();
		switch (r) {
			case 'c':
				computer_game_routine();
			case 'r':
				960_game_routine();
			default: 
				normal_game_routine();
			
		}
		PORTC ^= (1 << PC1);
		
		USART_transmit_str("Hello PC! the char i recieved was");
		USART_block_transmit(r);
		USART_block_transmit('\n');
		
		_delay_ms(1000);
	}
}