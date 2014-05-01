#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "lsm303.h"
#include "debug.h"

#define sleep() do { __asm__ __volatile__ ("sleep\n\t" ::); } while(0)

volatile int is_alarmOn = 0;

void delay_ms(uint16_t ms) {
    while (ms--) {
        _delay_ms(1);
    }
}

void pb_on(uint8_t pin) {
    PORTB |= (1 << pin);
}

void pb_off(uint8_t pin) {
    PORTB &= ~(1 << pin);
}

void pd_on(uint8_t pin) {
    PORTD |= (1 << pin);
}

void pd_off(uint8_t pin) {
    PORTD &= ~(1 << pin);
}


void led_init(void) {
    DDRB |= (1 << DDB0);
    DDRD |= (1 << DDD7);
}

void pb_blink(uint8_t pin, uint8_t times, uint8_t hz) {
    uint8_t i;
    uint16_t delay = 1000 / (2*hz);
    for (i = 0; i < times; i++) {
        pb_on(pin);
        delay_ms(delay);
        pb_off(pin);
        delay_ms(delay);
    }
}

void pd_blink(uint8_t pin, uint8_t times, uint8_t hz) {
    uint8_t i;
    uint16_t delay = 1000 / (2*hz);
    for (i = 0; i < times; i++) {
        pd_on(pin);
        delay_ms(delay);
        pd_off(pin);
        delay_ms(delay);
    }
}

void signal_start(void) {
    pb_blink(PB0, 1, 5);
}

void enable_interrupts(void) {
    // INT0
    // Make input
    DDRD &= ~(1 << DDD2);
    // Enable pull-up
    PORTD |= (1 << PD2);
    // Trigger on logic low (also allows INT0 to wake from sleep)
    MCUCR &= ~(3 << ISC00);
    // Turn on
    GICR |= (1 << INT0);

    // INT1
    DDRD &= ~(1 << DDD3);
    PORTD &= ~(1 << PD3);
    MCUCR |= (3 << ISC10);
    GICR |= (1 << INT1);

    // Enable global interrupts
    sei();
}

void alarm_on(void) {
    if (is_alarmOn == 0) {
        pb_blink(PB0, 2, 2);
        is_alarmOn = 1;
    } else {
        pb_blink(PB0, 1, 1);
    }
}

void alarm_off(void) {
    if (is_alarmOn == 1) {
        pb_blink(PB0, 3, 10);
        is_alarmOn = 0;
    }
}

// Interrupted by RF signal
ISR (INT0_vect) {
    // OK to pause interrupts long enough to atomically turn on or off
    if (bit_is_set(PINB, PB1)) {
        alarm_on();
    } else if (bit_is_set(PINB, PB2)) {
        alarm_off();
    }
}

// Interrupted by accelerometer data
ISR (INT1_vect) {
}

void rf_init(void) {
    DDRB &= ~(1 << DDB1);
    DDRB &= ~(1 << DDB2);
}

void accel_init(void) {
    lsm303_init();
}

volatile char *c = 0;;
void accel_print(void) {
    lsm303Data_t data = lsm303_read();
    
    if (c == 0) {
        c = (char*) malloc(32);
    }
    //dtostre(data.accel.x, c, 4, 0); debug(c);
    dtostre(data.accel.y, c, 4, 0); debug(c);
    //dtostre(data.accel.z, c, 4, 0); debug(c);
    //dtostre(data.mag.x, c, 4, 0); debug(c);
    //dtostre(data.mag.y, c, 4, 0); debug(c);
    //dtostre(data.mag.z, c, 4, 0); debug(c);
}
    
void debug(char* msg) {
    while (*msg) {
        usart_write8(*msg++);
    }
    usart_write8('\r');
    usart_write8('\n');
}

void debugi(uint8_t status) {
    char *c = (char*) malloc(5);
    itoa(status, c, 16);
    debug(c);
}

int main(void) {

    led_init();

    usart_init();
    debug("USART ON");

    rf_init();
    debug("RF ON");

    accel_init();
    debug("Accelerometer ON");

    enable_interrupts();
    debug("Interrupts ON");

    signal_start();

    while (1) {
        /*if (is_alarmOn == 0) {
            _delay_ms(200);
            MCUCR |= (1 << SE);
            MCUCR |= (1 << SM1);
            MCUCR &= ~((1 << SM0) | (1 << SM2));
            sleep();
        }*/
        // Echo serial
        //usart_write8(usart_read8());
        accel_print();
    }

    return 0;
}
