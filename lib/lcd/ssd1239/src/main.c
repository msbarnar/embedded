//
//  main.c
//  ssd1239
//
//  Created by Matthew Barnard on 4/23/14.
//
//

#include <util/delay.h>
#include "tft_gpio16.h"


int main(void) {
  
  tft_init();
  _delay_ms(10);
  tft_draw_fill(0);
  
  volatile int i;
  while (1) { ++i; }
  
  return 0;
}
