#include <avr/io.h>
#include <util/twi.h>
#include <stdlib.h>
#include "i2c.h"
#include "debug.h"

#define MAX_TRIES 50


void debug_status(uint8_t status) {
    char *c = (char*) malloc(5);
    itoa(status, c, 16);
    debug(c);
}

// Protocol described in ATMEGA8 datasheet p. 170
void i2c_init(void) {
    // TWBR: Bit rate register
    // TWPS: Prescale bits in TWI Status Register (TWSR)
    // SCL: F_CLK / (16 + 2(TWBR) * 4**TWPS))
    // No prescaler
    TWSR = 0;
    // SCL = 1,000,000 / (16 + 2(12) * 4**0) == 25,000
    TWBR = 1;
    // Enable I2C
    TWCR = (1 << TWEN);
}

uint8_t i2c_transmit(I2C_MODE mode) {
    switch (mode) {
        case I2C_START:
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
            break;
        case I2C_DATA_NACK:
            TWCR = (1 << TWINT) | (1 << TWEN);
            break;
        case I2C_DATA_ACK:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
            break;
        case I2C_STOP:
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            return 0;
    }

    // Wait for TWINT to be reset
    while (!(TWCR & (1 << TWINT)));

    // Return status
    return (TWSR & 0xF8);
}

volatile uint8_t open_sla;
volatile uint8_t is_open;

int8_t i2c_open(uint8_t sla, I2C_OPEN mode) {
    uint8_t try = 0;
    uint8_t status;

i2c_retry:
    if (try++ >= MAX_TRIES) return -1;

    status = i2c_transmit(I2C_START);

    if ((status == TW_MT_ARB_LOST) || (status == TW_MR_ARB_LOST)) {
        debug("I2C(W): starting: ARB_LOST");
        debug_status(status);
        goto i2c_retry;
    }
    if ((status != TW_START) && (status != TW_REP_START)) {
        debug("I2C(W): starting: failed");
        debug_status(status);
        return -1;
    }

    open_sla = sla;
    is_open = 1;

    // Send address and mode
    if (mode == I2C_WRITE) {
        TWDR = open_sla | TW_WRITE;
    } else if (mode == I2C_READ) {
        TWDR = open_sla | TW_READ;
    }

    status = i2c_transmit(I2C_DATA_NACK);
    
    if ((status == TW_MT_SLA_NACK) || (status == TW_MR_SLA_NACK) 
            || (status == TW_MT_ARB_LOST)) {
        debug("I2C: Sending address: SLA_NACK or ARB_LOST");
        debug_status(status);
        goto i2c_retry;
    }
    if (((mode == I2C_WRITE) && (status != TW_MT_SLA_ACK))
            || ((mode == I2C_READ) && (status != TW_MR_SLA_ACK))) {
        debug("I2C: Sending address: not SLA_ACK");
        debug_status(status);
        return -1;
    }

    return 1;
}

int8_t i2c_send(uint8_t *data, uint8_t len) {
    uint8_t status;
    int8_t retval = -1;

    // Send data bytes
    for (; len > 0; len--) {
        TWDR = *data++;
        status = i2c_transmit(I2C_DATA_ACK);
        if (status != TW_MT_DATA_ACK) {
            debug("I2C(W):  Sending data: not SLA_ACK");
            debug_status(status);
            return -1;
        }
    }

    // OK
    return 1;

    return retval;
}

void i2c_close() {
    i2c_transmit(I2C_STOP);
    open_sla = 0;
    is_open = 0;
}

int8_t i2c_recv(uint8_t *data, uint8_t len) {
    uint8_t try = 0;
    uint8_t status;
    int8_t retval = -1;

    if (try++ >= MAX_TRIES) return retval;

    for (; len > 0; len--) {
        // Read data
        if (len > 1) {
            status = i2c_transmit(I2C_DATA_ACK);
            if (status != TW_MR_DATA_ACK) {
                debug("I2C(R): Not ACK");
                debug_status(status);
                goto i2c_quit;
            }
        } else {
            status = i2c_transmit(I2C_DATA_NACK);
            if (status != TW_MR_DATA_NACK) {
                debug("I2C(R): Not NACK");
                debug_status(status);
                goto i2c_quit;
            }
        }

        *data++ = TWDR;
    }

    retval = (len == 0) ? 1 : -1;

i2c_quit:
    return retval;
}

