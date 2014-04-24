
/*
 Title: USART send
 Author: www.avrprojects.net
 date: 29 september 2012
 Software: WinAVR
 Hardware: AR attiny project board
 Target: Attiny2313
 Note: sends a byte to the PC*/
/* the general include files */
#include <avr/io.h>
#include <inttypes.h>
/* extra definitions */
#define FCPU 8000000L
#define BAUD 
// Define baud rate
#define USART_BAUD 9600L
#define USART_UBBR_VALUE ((FCPU/(USART_BAUD<<4))-1)
void USART_vInit(void)
{
// Set baud rate
UBRRH = (uint8_t)(USART_UBBR_VALUE>>8);
UBRRL = (uint8_t)USART_UBBR_VALUE;
// Set frame format to 8 data bits, no parity, 1 stop bit
UCSRC = (0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
// Enable receiver and transmitter
UCSRB = (1<<RXEN)|(1<<TXEN);
}
void USART_SendChar(uint8_t data)
{
// Wait if a byte is being transmitted
while((UCSRA&(1<<UDRE)) == 0);
// Transmit data
UDR = data;
}

void sendString(char s[]) { 
int i = 0; 
while(i < 64)
{ // don't get stuck if it is a bad string
if( s[i] == '\0' ) break; // quit on string terminator 
USART_SendChar(s[i++]); 
}
}

int main(void)
{
DDRB = 0xFF;
// Initialise USART
USART_vInit();
// Send string
sendString ("Hello World\n\r");
sendString ("How are you?\n\r");
}

