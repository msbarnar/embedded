#ifndef __KS0108_H__
#define __KS0108_H__

#include <avr/io.h>
#include <inttypes.h>

/* Data/Instruction */
#define LCD_DI        PD0
#define LCD_DI_PORT   PORTD
#define LCD_DI_DDR    DDRD
#define LCD_DI_OUT    LCD_DI_DDR |= (1 << LCD_DI)
#define LCD_DI_DATA   LCD_DI_PORT |= (1 << LCD_DI)
#define LCD_DI_INSTR  LCD_DI_PORT &= ~(1 << LCD_DI)

/* Data Read/Write */
#define LCD_RW        PD1
#define LCD_RW_PORT   PORTD
#define LCD_RW_DDR    DDRD
#define LCD_RW_OUT    LCD_RW_DDR |= (1 << LCD_RW)
#define LCD_RW_READ   LCD_RW_PORT |= (1 << LCD_RW)
#define LCD_RW_WRITE  LCD_RW_PORT &= ~(1 << LCD_RW)

/* H->L Enable */
#define LCD_E         PD2
#define LCD_E_PORT    PORTD
#define LCD_E_PIN     PIND
#define LCD_E_DDR     DDRD
#define LCD_E_OUT     LCD_E_DDR |= (1 << LCD_E)
#define LCD_E_ENABLE  LCD_E_PORT |= (1 << LCD_E)
#define LCD_E_DISABLE LCD_E_PORT &= ~(1 << LCD_E)
#define LCD_E_STROBE  LCD_E_ENABLE; LCD_WAIT; LCD_E_DISABLE

/* Chip Select 1 */
#define LCD_CS0       PD3
#define LCD_CS0_PORT  PORTD
#define LCD_CS0_DDR   DDRD
#define LCD_CS0_OUT   LCD_CS0_DDR |= (1 << LCD_CS0)
#define LCD_CS0_ENABLE  LCD_CS1_DISABLE; LCD_CS0_PORT |= (1 << LCD_CS0)
#define LCD_CS0_DISABLE   LCD_CS0_PORT &= ~(1 << LCD_CS0)

/* Chip Select 2 */
#define LCD_CS1       PD4
#define LCD_CS1_PORT  PORTD
#define LCD_CS1_DDR   DDRD
#define LCD_CS1_OUT   LCD_CS1_DDR |= (1 << LCD_CS1)
#define LCD_CS1_ENABLE  LCD_CS0_DISABLE; LCD_CS1_PORT |= (1 << LCD_CS1)
#define LCD_CS1_DISABLE   LCD_CS1_PORT &= ~(1 << LCD_CS1)

/* ~RESET */
#define LCD_RST       PD5
#define LCD_RST_PORT  PORTD
#define LCD_RST_DDR   DDRD
#define LCD_RST_OUT   LCD_RST_DDR |= (1 << LCD_RST)
#define LCD_RST_HIGH  LCD_RST_PORT |= (1 << LCD_RST)
#define LCD_RST_LOW   LCD_RST_PORT &= ~(1 << LCD_RST)

/* 8bit Parallel Data Bus */
#define LCD_DB_PORT   PORTB
#define LCD_DB_PIN    PINB
#define LCD_DB_DDR    DDRB
#define LCD_DB_OUT    LCD_DB_DDR = 0xFF
#define LCD_DB_IN     LCD_DB_DDR = 0; LCD_DB_PORT = 0

#define LCD_WAIT      __asm("nop\n\tnop\n\tnop")

#define LCD_WIDTH     128
#define LCD_HEIGHT    64

#define LCD_STATUS_BUSY 0x80

#define LCD_CMD_PAGE    0xB8
#define LCD_CMD_ADDR    0x40
#define LCD_CMD_START   0xC0
#define LCD_CMD_PWRON   0x3F
#define LCD_CMD_PWROFF  0x3E

#define LCD_CURRENT_CONTROLLER  screen_x / LCD_HEIGHT
#define LCD_NUMCONTROLLERS 2


void lcd_init(void);
void lcd_control(uint8_t controller);
void lcd_done(void);
uint8_t lcd_status(uint8_t controller);
void lcd_command(uint8_t command, uint8_t controller);
void lcd_data(uint8_t data);
uint8_t lcd_read(void);

void lcd_move(uint8_t x, uint8_t y);
void lcd_pixel(uint8_t x, uint8_t y, uint8_t color);
void lcd_clear(uint8_t color);

#endif

