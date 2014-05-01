#include <avr/io.h>
#include <util/delay.h>
#include "ks0108.h"

int main(void) {
  lcd_init();

  while (1) {
    lcd_clear(0);
    _delay_ms(500);
    lcd_clear(0xFF);
    _delay_ms(500);
  }

  return 0;
}
