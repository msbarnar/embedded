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

/**
 * Write an 8 bit value to a specific register of the LSM303 at `address`.
 */
int write8(uint8_t address, uint8_t reg, uint8_t value) {
  if (i2c_send(address, &reg, 1) == 0xFFFFFFFF) return -1;
  if (i2c_send(address, &value, 1) == 0xFFFFFFFF) return -1;

  return 0;
}

/**
 * Read an 8 bit value from a specific register for the LSM303 at `address`.
 */
uint8_t read8(uint8_t address, uint8_t reg) {
  uint8_t value;

  if (i2c_send(address, &reg, 1) == 0xFFFFFFFF) return -1;
  if (i2c_recv(address, &value, 1) == 0xFFFFFFFF) return -1;

  return value;
}

uint32_t lsm303_begin() {
  // Enable I2C
  i2c_init(MODE_100kbps, 3);

  // Enable accelerometer
  if (write8(LSM303_ADDRESS_ACCEL, LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27) < 0)
    return -1;

  // Enable the magnetometer
  if (write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_MR_REG_M, 0x00) < 0)
    return -1;

  return 0;
}

lsm303Data_t lsm303_read() {
  lsm303Data_t data;

  uint8_t buf[6] = {0};

  // Read the accelerometer
  buf[0] = LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80;
  i2c_send(LSM303_ADDRESS_ACCEL, buf, 1);
  i2c_recv(LSM303_ADDRESS_ACCEL, buf, 6);

  // x = (xlo | (xhi << 8)) >> 4;
  data.accel.x = (buf[0] | (buf[1] << 8)) >> 4;
  // y = (ylo | (yhi << 8)) >> 4;
  data.accel.y = (buf[2] | (buf[3] << 8)) >> 4;
  // z = (zlo | (zhi << 8)) >> 4;
  data.accel.z = (buf[4] | (buf[5] << 8)) >> 4;

  // Read the magnetometer
  buf[0] = LSM303_REGISTER_MAG_OUT_X_H_M;
  i2c_send(LSM303_ADDRESS_MAG, buf, 1);
  i2c_recv(LSM303_ADDRESS_MAG, buf, 6);

  // high before low
  data.mag.x = buf[1] | (buf[0] << 8);
  data.mag.y = buf[3] | (buf[2] << 8);
  data.mag.z = buf[5] | (buf[4] << 8);

  // TODO: Calculate orientation
  data.mag.orientation = 0.0;

  return data;
}

void lsm303_setMagGain(lsm303MagGain_t gain) {
  write8(LSM303_ADDRESS_MAG, LSM303_REGISTER_MAG_CRB_REG_M, (uint8_t) gain);
}
