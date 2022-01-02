#ifndef USART_H_
#define USART_H_

#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
#define BAUDRATE 9600
#define UBRR (F_CPU/(BAUDRATE<<4))-1

void USART_init(unsigned int baud);
void USART_block_transmit(unsigned char data);
unsigned char USART_noblock_transmit(unsigned char data);
void USART_transmit_str(const char* str);
unsigned char USART_block_recieve();
unsigned char USART_noblock_recieve();

#endif /* USART_H_ */