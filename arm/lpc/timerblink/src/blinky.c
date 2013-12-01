/*
===============================================================================
 Name        : blinky.c
 Description : Blinks LED2 on the LPC1769 development board
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

// Timer control
enum {
  PCONP_TIM0 = 1,
  PCONP_GPIO = 15,

  PIN_LED2 = 1 << 22,

  GPIO_LED2 = 3 << 12,

  CCLK_OVER_4 = 0,
  CCLK = 1 << 0,
  CCLK_OVER_2 = 1 << 1,
  CCLK_OVER_8 = 3,

  MCR_INT_MR0 = 1 << 0,
  MCR_INT_MR0_AND_RESET = MCR_INT_MR0 | (1 << 1),

  TCR_START = 1 << 0,
  TCR_RESET = 1 << 1,

  INTERRUPTEDBY_MR0 = 0x01,
};

void init_timer();

int main(void) {
  // Power up GPIO
  // Table 46 p. 63
  LPC_SC->PCONP |= (1 << PCONP_GPIO);
  // Set the operating mode of the pin to GPIO (default, powered IO port)
  // PINSEL0 is P0.0->P0.15, PINSEL1 is P0.16->P0.28
  // Set bits 13:12 to 0 for GPIO on P0.22
  // (user guide s8.5.2 p. 108)
  LPC_PINCON->PINSEL1 &= (~GPIO_LED2);
  // Set the direction of the GPIO P0.22 to OUTPUT (s9.5 pp. 122-123)
  LPC_GPIO0->FIODIR |= PIN_LED2;

  // Start the timer
  init_timer();

  // Force the counter to be placed into memory
  volatile static uint32_t i = 0 ;
  while(1) {
    i++;
  }
  return 0;
}

void init_timer() {
  // Power Control for Peripherals
  // Allows disabling unused peripherals for power saving
  // s4.8.6 p. 60
  LPC_SC->PCONP |= (1 << PCONP_TIM0);    // Power up Timer0
  // Clock dividers - Peripheral Clock Selection
  // Bits 3:2 PCLK_TIMER0: Controls the clock signal to the peripheral timer
  // s4.7.3 pp. 56-57
  LPC_SC->PCLKSEL0 |= CCLK_OVER_2 << 2; // Timer clock = CCLK/2

  // Match Registers are continuously compared to Timer Counter; when equal, actions
  // can be triggered automatically, such as triggering an interrupt.
  // Clock rate = 100MHz, and CCLK_OVER_2 so timer rate is 50MHz
  // so 1 second = 50Mticks
  LPC_TIM0->MR0 = 50000000;
  // s21.6.8 p. 496 (table 429: match control registers)
  LPC_TIM0->MCR |= MCR_INT_MR0_AND_RESET;    // Interrupt when MR0 == TC0
  // s21.6.2 p. 494 (table 427: timer control register)
  LPC_TIM0->TCR |= TCR_RESET;  // Reset Timer0
  // "The counters remain reset until TCR is returned to zero." (p. 494)
  LPC_TIM0->TCR = 0;

  // core_cm3.h from LPC17xx.h
  // TIMER0_IRQHandler needs to be defined (and extern "C" if in C++)
  NVIC_EnableIRQ(TIMER0_IRQn);// Set timer interrupt

  LPC_TIM0->TCR |= TCR_START;    // Start Timer0
}

/**
 * Called when TIM0->TC == TIM0->MR0
 */
void TIMER0_IRQHandler(void) {
  // Interrupts can be triggered for multiple reasons; make sure this was triggerred
  // because of MR0
  // IR identifies which of 8 interrupt sources triggered this
  // s21.6.1 p. 493
  if ((LPC_TIM0->IR & INTERRUPTEDBY_MR0) > 0) {
    // Clear the interrupt flag so it doesn't fire again right away
    LPC_TIM0->IR |= INTERRUPTEDBY_MR0;
    // Toggle the LED
    // FIOPIN reads and writes according to FIOMASK
    // s9.5 p. 122
    LPC_GPIO0->FIOPIN ^= PIN_LED2;
  }
}
