/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#if defined (__CODE_RED)
#include <NXP/crp.h>
// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#endif

#include <stdio.h>
#include "lsm303.h"

int main() {
    lsm303_begin();

    volatile int i;

    for (;;) {
      for (i = 0; i < 2000000; i++) {
        lsm303Data_t data = lsm303_read();
      }
    }

    return 0;
}
