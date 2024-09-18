// Switch.c
// Runs on LM4F120/TM4C123
// Switch interface for alarm clock
// September 17, 2024

// PF0 is mode select
// PF4 is display select
// PC6 is left
// PC5 is right 
// PC4 is up
// PC7 is down
// if alarm is sounding, any button will quiet the alarm

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "Switch.h"

#define PF0  (*((volatile uint32_t *)0x40025004))
#define PF4  (*((volatile uint32_t *)0x40025040))
#define PC4  (*((volatile uint32_t *)0x40006040)) //For bit banding
#define PC5  (*((volatile uint32_t *)0x40006080)) //For bit banding
#define PC6  (*((volatile uint32_t *)0x40006100)) //For bit banding
#define PC7  (*((volatile uint32_t *)0x40006200)) //For bit banding
// ---------- Prototypes   -------------------------

void Switch_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x04;        // 1) activate port C
  while((SYSCTL_PRGPIO_R&0x04)==0){};   // allow time for clock to stabilize
  GPIO_PORTC_AMSEL_R &= ~0xF0;      // 2) disable analog functionality on PC4-7
  GPIO_PORTC_DIR_R &= ~0xF0;         // 4) make PC4-7 in
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF0FFFFF); //GPIO
	GPIO_PORTC_AFSEL_R &= ~0xF0;      // 5) regular port function
  GPIO_PORTC_DEN_R |= 0xF0;         // 6) enable digital I/O on PC4-7
		
	SYSCTL_RCGCGPIO_R |= 0x00000020;  // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20) == 0){};// allow time for clock to stabilize
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
  GPIO_PORTF_DATA_R = 0;            // LEDs off
}

uint8_t Mode_In(void){ //mode|display
	return (PF0|(PF4 >> 3));
}

uint8_t Direction_In(void){ //up|right|left|down
	return ((PC4 >> 2)|(PC5 >> 2)|(PC6 >> 2)|(PC7 >> 2));
}


