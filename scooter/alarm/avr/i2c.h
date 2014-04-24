#ifndef __I2C_H__
#define __I2C_H__

typedef enum {
    I2C_START,
    I2C_DATA_NACK,
    I2C_DATA_ACK,
    I2C_STOP
} I2C_MODE;

typedef enum {
    I2C_READ,
    I2C_WRITE
} I2C_OPEN;

void i2c_init(void);
int8_t i2c_open(uint8_t sla, I2C_OPEN mode);
int8_t i2c_send(uint8_t *data, uint8_t len);
int8_t i2c_recv(uint8_t *data, uint8_t len);
void i2c_close();

#endif

