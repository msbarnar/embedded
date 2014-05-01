/*
*  tft_gpio16.c
*  ssd1289
*
*  Created by Matthew Barnard on 4/23/14.
*
*/

#include "tft_gpio16.h"
#include <util/delay.h>

void tft_draw_fill(uint16_t color) {
  uint32_t i;
  
  TFT_CS_LOW;
  
  tft_address(0, 0, 239, 319);
  tft_data(color);
  
  for (i = 0; i < 38121; ++i) {
    TFT_WR_LOW;
    TFT_WR_HIGH;
    TFT_WR_LOW;
    TFT_WR_HIGH;
  }
  
  TFT_CS_HIGH;
}


void tft_init(void) {
  /* enable output on the control ports */
  TFT_RS_DDR |=   (1 << TFT_RS);
  TFT_CS_DDR |=   (1 << TFT_CS);
  TFT_WR_DDR |=   (1 << TFT_WR);
  TFT_RD_DDR |=   (1 << TFT_RD);
  TFT_RST_DDR |=  (1 << TFT_RST);
  TFT_DATA0_DDR = 0xFF;
  TFT_DATA1_DDR = 0xFF;
  TFT_DATA0_PORT = 0;
  TFT_DATA1_PORT = 0;
  
  TFT_RD_HIGH;
  TFT_RST_HIGH;
  
  _delay_ms(5);
  
  TFT_RST_LOW;
  
  _delay_ms(15);
  
  TFT_RST_HIGH;
  
  _delay_ms(15);
  
  TFT_CS_LOW;
  
  /* turn on oscillator */
  tft_cdata(0x0000, 0x0001);
  /* power control 0..4 */
  tft_cdata(0x0003, 0xA8A4);
  tft_cdata(0x000C, 0x0000);
  tft_cdata(0x000D, 0x800C);
  tft_cdata(0x000E, 0x2B00);
  tft_cdata(0x001E, 0x00B7);
  tft_cdata(0x0001, 0x2B3F);  /* driver output control */
  tft_cdata(0x0002, 0x0600);  /* LCD driver waveform */
  tft_cdata(0x0010, 0x0000);  /* sleep mode */
  tft_cdata(0x0011, 0x6070);  /* entry mode */
  tft_cdata(0x0005, 0x0000);  /* compare register */
  tft_cdata(0x0006, 0x0000);  /* compare register */
  tft_cdata(0x0016, 0xEF1C);  /* horizontal porch */
  tft_cdata(0x0017, 0x0003);  /* vertical porch */
  tft_cdata(0x0007, 0x0233);  /* display control */
  tft_cdata(0x000B, 0x0000);  /* frame cycle control */
  tft_cdata(0x000F, 0x0000);  /* gate scan start position */
  tft_cdata(0x0041, 0x0000);  /* vertical scroll control */
  tft_cdata(0x0042, 0x0000);  /* horizontal scroll control */
  tft_cdata(0x0048, 0x0000);  /* first window start */
  tft_cdata(0x0049, 0x013F);  /* first window end */
  tft_cdata(0x004A, 0x0000);  /* second window start */
  tft_cdata(0x004B, 0x0000);  /* second window end */
  tft_cdata(0x0044, 0xEF95);  /* horizontal RAM addr pos */
  tft_cdata(0x0045, 0x0000);  /* vertical RAM addr start pos */
  tft_cdata(0x0046, 0x013F);  /* vertical RAM addr end pos */
  /* gamma control */
  tft_cdata(0x0030, 0x0707);
  tft_cdata(0x0031, 0x0204);
  tft_cdata(0x0032, 0x0204);
  tft_cdata(0x0033, 0x0502);
  tft_cdata(0x0034, 0x0507);
  tft_cdata(0x0035, 0x0204);
  tft_cdata(0x0036, 0x0204);
  tft_cdata(0x0037, 0x0502);
  tft_cdata(0x003A, 0x0302);
  tft_cdata(0x003B, 0x0302);
  /* RAM write data mask */
  tft_cdata(0x0023, 0x0000);
  tft_cdata(0x0024, 0x0000);
  tft_cdata(0x004f, 0x0000);  /* GDDRAM Y addr counter */
  tft_cdata(0x004e, 0x0000);  /* GDDRAM X addr counter */
  
  tft_command(0x0022);        /* RAM data write */

  TFT_CS_HIGH;
}


void tft_command(uint16_t command) {
  TFT_RD_HIGH;
  /* what is RS? low for commands, high for data */
  TFT_RS_LOW;
  
  /* set the data pins */
  /* endianness? */
  TFT_DATA0_PORT = command;
  TFT_DATA1_PORT = (command >> 8);
  
  /* clock the WR port */
  TFT_WR_LOW;
  TFT_WR_HIGH;
}


void tft_data(uint16_t data) {
  TFT_RD_HIGH;
  /* RS is high for data and low for commands */
  TFT_RS_HIGH;
  
  /* endianness? */
  TFT_DATA0_PORT = data;
  TFT_DATA1_PORT = (data >> 8);
  
  /* clock the WR port */
  TFT_WR_LOW;
  TFT_WR_HIGH;
}

void tft_cdata(uint16_t command, uint16_t data) {
  TFT_CS_LOW;
  
  tft_command(command);
  tft_data(data);
  
  TFT_CS_HIGH;
}

void tft_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  tft_cdata(0x0044, (x2 << 8) + x1);  /* column addr start 2 */
  tft_cdata(0x0045, y1);              /* column addr start 1 */
  tft_cdata(0x0046, y2);              /* column addr end 2 */
  tft_cdata(0x004e, x1);              /* column addr end 1 */
  tft_cdata(0x004f, y1);              /* row addr start 2 */
  tft_command(0x0022);
}