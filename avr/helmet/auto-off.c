/*
 * auto-off.c
 *
 * Pulls PB7 high when turned on. Lets PB7 go low to turn off after some idle
 * duration.
 * Pulls PB0 high to demonstrate it's on via LED or something.
 *
 * Attach PB7 to the base of a transistor with the collector to 3-5V and the
 * emitter to VCC. Then attach a momentary switch between the base and source
 * voltage. When the switch is activated the transistor will be kept high until
 * auto off.
 *
 * The switch can also be run to an input pin for manual off, though this may
 * present a race condition where the MCU turning off while the switch is
 * depressed.
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    char pin_signal = _BV(0);
    char pin_on = _BV(7);

    /* Pull PB7 high to keep on */
    DDRB = pin_signal | pin_on;
    PORTB = pin_on;

    /* Pull PB0 high to signal we're on */
    PORTB |= pin_signal;

    /* Wait a while */
    _delay_ms(4000);

    /*
     * Try to turn off until we actually do.
     * If the user holds the button down we won't turn off! so keep trying,
     * she'll give up eventually. */
    while (1) {
        PORTB = 0;
    }

    return 0;
}
