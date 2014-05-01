//
//  tft_gpio16.h
//  ssd1239
//
//  Created by Matthew Barnard on 4/23/14.
//
//

#ifndef tft_gpio16_h
#define tft_gpio16_h

#include <inttypes.h>
#include <avr/io.h>

#define TFT_CS        PC0
#define TFT_CS_PORT   PORTC
#define TFT_CS_DDR    DDRC
#define TFT_CS_HIGH   TFT_CS_PORT |= (1 << TFT_CS)
#define TFT_CS_LOW    TFT_CS_PORT &= ~(1 << TFT_CS)

#define TFT_RS        PC1
#define TFT_RS_PORT   PORTC
#define TFT_RS_DDR    DDRC
#define TFT_RS_HIGH   TFT_RS_PORT |= (1 << TFT_RS)
#define TFT_RS_LOW    TFT_RS_PORT &= ~(1 << TFT_RS)

#define TFT_WR        PC2
#define TFT_WR_PORT   PORTC
#define TFT_WR_DDR    DDRC
#define TFT_WR_HIGH   TFT_WR_PORT |= (1 << TFT_WR)
#define TFT_WR_LOW    TFT_WR_PORT &= ~(1 << TFT_WR)

#define TFT_RD        PC3
#define TFT_RD_PORT   PORTC
#define TFT_RD_DDR    DDRC
#define TFT_RD_HIGH   TFT_RD_PORT |= (1 << TFT_RD)
#define TFT_RD_LOW    TFT_RD_PORT &= ~(1 << TFT_RD)

#define TFT_RST       PC5
#define TFT_RST_PORT  PORTC
#define TFT_RST_DDR   DDRC
#define TFT_RST_HIGH  TFT_RST_PORT |= (1 << TFT_RST)
#define TFT_RST_LOW   TFT_RST_PORT &= ~(1 << TFT_RST)

/* 16 bit data bus */
/* bits 0..7 */
#define TFT_DATA0_PORT  PORTB
#define TFT_DATA0_DDR   DDRB
/* bits 8..15 */
#define TFT_DATA1_PORT  PORTD
#define TFT_DATA1_DDR   DDRD


void tft_init(void);
void tft_command(uint16_t command);
void tft_data(uint16_t data);
void tft_cdata(uint16_t command, uint16_t data);
void tft_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void tft_draw_fill(uint16_t color);

#endif
