/*
 blinklight.c
 
 Blinks a LED- basic example to show you can upload to your avr
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LEFTBLINKER _BV(3)
#define STOPLIGHT _BV(2)
#define RIGHTBLINKER _BV(1)
#define FADE_RATE 1
#define INITIAL_WIDTH 255
#define MIN_WIDTH 10


void fade_out(char bit) {
    unsigned char pulse_width = INITIAL_WIDTH;
    unsigned char rate = FADE_RATE;
    unsigned char count;

    while (pulse_width > MIN_WIDTH) {
        /* On */
        PORTB |= bit;

        /* Count out pulse */
        for (count = 0; count < pulse_width; count++) {
            asm("NOP");
        }

        /* Off */
        PORTB &= ~bit;

        for (; count; count++) {
            asm("NOP");
        }

        if (!rate--) {
            pulse_width--;
            rate = FADE_RATE;
        }
    }
}

void fade_in(char bit) {
    unsigned char pulse_width = MIN_WIDTH;
    unsigned char rate = FADE_RATE;
    unsigned char count;

    while (pulse_width < INITIAL_WIDTH) {
        /* On */
        PORTB |= bit;

        /* Count out pulse */
        for (count = 0; count < pulse_width; count++) {
            asm("NOP");
        }

        /* Off */
        PORTB &= ~bit;

        for (; count; count++) {
            asm("NOP");
        }

        if (!rate--) {
            pulse_width++;
            rate = FADE_RATE;
        }
    }
}



void stoplight(void) {
    PORTB |= STOPLIGHT;
    _delay_ms(1000);
    PORTB &= ~STOPLIGHT;
}

void blink(char blinker) {
    /* Pulse blinker */
    int i;
    for (i = 0; i < 1; i++) {
        fade_in(blinker);
        fade_out(blinker);
    } 
    PORTB &= ~blinker;
}

int main(void) {
    /* Enable pins PB1 and PB3 for output via PORTB */
    DDRB = STOPLIGHT | LEFTBLINKER | RIGHTBLINKER;
    
    for (;;) {
        blink(LEFTBLINKER);
        stoplight();
        blink(RIGHTBLINKER);
        stoplight();
    }

    return 0;
}
