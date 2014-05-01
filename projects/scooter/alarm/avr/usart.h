#ifndef __USART_H__
#define __USART_H__

// The internal oscillator is only precise enough for 1200baud
#define USART_BAUDRATE 1200 
#define USART_BAUD_PRESCALE ((F_CPU / (USART_BAUDRATE * 16UL)) - 1)

void usart_init();
void usart_write8(uint8_t b);
uint8_t usart_read8(void);

#endif
