#include "ks0108.h"
#include <util/delay.h>


uint8_t  screen_x;
uint8_t  screen_y;


void lcd_init(void) {
  LCD_DI_OUT;
  LCD_RW_OUT;
  LCD_E_OUT;
  LCD_CS0_OUT;
  LCD_CS1_OUT;
  LCD_RST_OUT;
  LCD_DB_OUT;

  LCD_CS0_DISABLE;
  LCD_CS1_DISABLE;

  lcd_command(LCD_CMD_PWRON, 0);
  lcd_command(LCD_CMD_PWRON, 1);
}

inline void lcd_control(uint8_t controller) {
  switch (controller) {
    case 0: LCD_CS0_ENABLE; break;
    case 1: LCD_CS1_ENABLE; break;
  }
}

inline void lcd_done(void) {
  LCD_E_DISABLE; 
  LCD_CS0_DISABLE;
  LCD_CS1_DISABLE;
}

uint8_t lcd_status(uint8_t controller) {
  uint8_t status;
  /* Set data bus to input */
  LCD_DB_IN;
  /* Enable read */
  LCD_RW_READ;
  LCD_DI_INSTR;
  /* Select controller */
  lcd_control(controller);
  /* Enable controller */
  LCD_E_ENABLE;
  /* Wait and read status */
  LCD_WAIT;
  status = LCD_DB_PIN;

  /* Disable controller */
  lcd_done();

  return status;
}

void lcd_command(uint8_t command, uint8_t controller) {
  /* Wait until not busy */
  while (lcd_status(controller) & LCD_STATUS_BUSY);

  /* Enable command write */
  LCD_DB_OUT;
  LCD_RW_WRITE;
  LCD_DI_INSTR;
  /* Select controller */
  lcd_control(controller);
  /* Write command to data bus */
  LCD_DB_PORT = command;
  /* Enable controller, causing it to read command from data bus */
  LCD_E_ENABLE;
  /* Give controller time to read command, then turn it off */
  LCD_WAIT;
  lcd_done();
}

void lcd_data(uint8_t data) {
  /* Wait until controller not busy */
  while (lcd_status(LCD_CURRENT_CONTROLLER) & LCD_STATUS_BUSY);

  /* Enable data write */
  LCD_DB_OUT;
  LCD_RW_WRITE;
  LCD_DI_DATA;
  /* Write data to bus */
  LCD_DB_PORT = data;
  /* Enable controller */
  lcd_control(LCD_CURRENT_CONTROLLER);
  LCD_E_ENABLE;
  /* Wait for controller to read data and disable */
  LCD_WAIT;
  lcd_done();
  /* Advance column */
  ++screen_x;
}

uint8_t lcd_read(void) {
  uint8_t data;
  /* Wait for controller */
  while(lcd_status(LCD_CURRENT_CONTROLLER) & LCD_STATUS_BUSY);
  /* Set to read */
  LCD_DB_IN;
  LCD_RW_READ;
  LCD_DI_DATA;
  /* Enable controller and wait for it to provide data */
  lcd_control(LCD_CURRENT_CONTROLLER);
  LCD_E_ENABLE;
  LCD_WAIT;
  /* Get data from parallel bus */
  data = LCD_DB_PIN;
  /* Disable controller */
  lcd_done();

  return data;
}

/* Selects the appropriate memory page for the specified pixel coordinates,
 * starting at the top left corner (0, 0).
 */
void lcd_move(uint8_t x, uint8_t y) {
  screen_x = x;
  screen_y = y;

  /* Each horizontal half of the screen is divided into eight 1-byte pages */
  /* Each page is 8 rows */
  /* E.g. the 63rd row is on the last page */
  lcd_command(LCD_CMD_PAGE | (y / 8),           LCD_CURRENT_CONTROLLER);
  lcd_command(LCD_CMD_ADDR | (x % LCD_HEIGHT),  LCD_CURRENT_CONTROLLER);
  lcd_command(LCD_CMD_START,                    LCD_CURRENT_CONTROLLER);
}

void lcd_pixel(uint8_t x, uint8_t y, uint8_t color) {
  uint8_t cbyte;
  /* Read in existing column */
  lcd_move(x, y);
  cbyte = lcd_read();
  /* Set specified pixel in column to color */
  /* Default organization has the least significant bit toward the bottom
   * of the screen, but we flip it so the 0th bit of the 0th page is the
   * top left pixel */
  if (color == BLACK) {
    cbyte &= (1 << (8 - (y % 8)));
  } else {
    cbyte |= (1 << (8 - (y % 8)));
  }
  /* Write back column */
  lcd_data(cbyte);
}

/* Clears the screen as fast as we can */
void lcd_clear(uint8_t color) {
  uint8_t i, j, k;
  /* For every vertical page the screen (a page contains 8 vertical pixels) */
  for (j = 0; j < 8; ++j) {
    /* Enable command write */
    LCD_DB_OUT;
    LCD_RW_WRITE;
    LCD_DI_INSTR;
    /* Write ADDR command to both controllers to put them both at column 0 */
    LCD_DB_PORT = LCD_CMD_ADDR;
    LCD_CS0_ENABLE;
    LCD_E_STROBE;
    LCD_CS1_ENABLE;
    LCD_E_STROBE;
    /* Write PAGE command to put each controller at the right row */
    LCD_DB_PORT = LCD_CMD_PAGE | j;
    LCD_CS0_ENABLE;
    LCD_E_STROBE;
    LCD_CS1_ENABLE;
    LCD_E_STROBE;
    /* Write START command to enable row */
    LCD_DB_PORT = LCD_CMD_START;
    LCD_CS1_ENABLE;
    LCD_E_STROBE;
    LCD_CS0_ENABLE;
    LCD_E_STROBE;

    /* Clear all 8 vertical pixels in that row */
    /* Write a 0 to the data port, then toggle Enable 128 times to
    * write that data value for each column */
    /* Enable data write */
    LCD_DI_DATA;
    /* Write data to bus */
    LCD_DB_PORT = color;
    /* Write across each controller in series */
    /* Controller 0 is already active */
    /* For each column, set E, wait, clear E */
    for (i = 0; i < 2; ++i) {
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
    }

    LCD_CS1_ENABLE;
    for (i = 0; i < 2; ++i) {
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
      LCD_E_STROBE;
    }
  }
}

