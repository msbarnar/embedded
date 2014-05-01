#include <avr/io.h>
#include "usart.h"

void usart_init() {
    // Set the clock rate
    UBRRH = (USART_BAUD_PRESCALE >> 8);
    UBRRL = USART_BAUD_PRESCALE;
    // Asynchronous, no parity, 1 stop bit, 8bpp
    UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
    // TX and RX
    UCSRB = (1 << TXEN) | (1 << RXEN);
}

void usart_write8(uint8_t b) {
    // Wait for the TX line to become available
    while ((UCSRA & (1 << UDRE)) == 0);
    UDR = b;
}

uint8_t usart_read8(void) {
    // Wait for a byte to become available
    while ((UCSRA & (1 << RXC)) == 0);
    return UDR;
}

