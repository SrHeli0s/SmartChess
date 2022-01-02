#include "usart.h"
#include <xc.h>

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
uint8_t USART_noblock_transmit(unsigned char data) {
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