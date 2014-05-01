/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  Modified for the LPC17xx by Matthew barnard.

  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "lsm303.h"
#include "i2c.h"
#include "debug.h"

/**
 * Write an 8 bit value to a specific register of the LSM303 at `address`.
 */
int8_t _lsm303_write8(uint8_t address, uint8_t reg, uint8_t value) {
    if (i2c_open(address, I2C_WRITE)) {
        if (i2c_send(&reg, 1)) {
            if (i2c_send(&value, 1)) {
                i2c_close();
                return 1;
            }
        }
    }
    return -1;
}

/**
 * Read an 8 bit value from a specific register for the LSM303 at `address`.
 */
int8_t _lsm303_read8(uint8_t address, uint8_t reg) {
  uint8_t value;

  if (i2c_open(address, I2C_WRITE)) {
    if (i2c_send(&reg, 1)) {
        i2c_open(address, I2C_READ);
        i2c_recv(&value, 1);
        i2c_close();
        return value;
    }
  }
  
  return -1;
}

int32_t lsm303_init() {
  // Enable I2C
  i2c_init();

  // Enable accelerometer
  if (_lsm303_write8(LSM303_ADDRESS_ACCEL, 
              LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x47) < 0)
    return -1;

  // Enable the magnetometer
  if (_lsm303_write8(LSM303_ADDRESS_MAG, 
              LSM303_REGISTER_MAG_MR_REG_M, 0x00) < 0)
    return -1;

  return 0;
}

lsm303Data_t lsm303_read() {
  lsm303Data_t data;

  uint8_t buf[6] = {0};

  // Read the accelerometer
  buf[0] = LSM303_REGISTER_ACCEL_OUT_X_L_A | (1 << 7);
  i2c_open(LSM303_ADDRESS_ACCEL, I2C_WRITE);
  i2c_send(buf, 1);
  i2c_open(LSM303_ADDRESS_ACCEL, I2C_READ);
  i2c_recv(buf, 6);
  i2c_close();

  // x = (xlo | (xhi << 8)) >> 4;
  data.accel.x = (buf[0] | (buf[1] << 8)) >> 4;
  // y = (ylo | (yhi << 8)) >> 4;
  data.accel.y = (buf[2] | (buf[3] << 8)) >> 4;
  // z = (zlo | (zhi << 8)) >> 4;
  data.accel.z = (buf[4] | (buf[5] << 8)) >> 4;

  // Read the magnetometer
  buf[0] = LSM303_REGISTER_MAG_OUT_X_H_M | (1 << 7);
  i2c_open(LSM303_ADDRESS_MAG, I2C_WRITE);
  i2c_send(buf, 1);
  i2c_open(LSM303_ADDRESS_MAG, I2C_READ);
  i2c_recv(buf, 6);
  i2c_close();

  // high before low
  data.mag.x = buf[1] | (buf[0] << 8);
  data.mag.z = buf[3] | (buf[2] << 8);
  data.mag.y = buf[5] | (buf[4] << 8);

  // TODO: Calculate orientation
  data.mag.orientation = 0.0;

  return data;
}

void lsm303_setMagGain(lsm303MagGain_t gain) {
  _lsm303_write8(LSM303_ADDRESS_MAG, 
          LSM303_REGISTER_MAG_CRB_REG_M, (uint8_t) gain);
}
