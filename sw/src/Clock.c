// Clock.c
// Runs on LM4F120/TM4C123
// Timer interface for alarm clock
// September 17, 2024

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "../inc/SysTickInts.h"
#include "Switch.h"

#define PD0  (*((volatile uint32_t *)0x4005B004))

uint8_t hour;
uint8_t minute;
uint8_t second;
bool daytime;


void Update_Time (void){
	second++;
	if(second == 60){
		second = 0;
		minute++;
		if(minute == 60){
			minute = 0;
			hour++;
			if(hour == 13){
				hour = 1;
				daytime ^= 1;
			} 
		}
	}
}

void Play_Sound(void){
	PD0 ^= 1;
}

void Sound_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x08;        // 1) activate port D
  while((SYSCTL_PRGPIO_R&0x08)==0){};   // allow time for clock to stabilize
  GPIO_PORTD_AMSEL_R &= ~0x01;      // 2) disable analog functionality on PD0
  GPIO_PORTD_DIR_R |= 0x01;         // 4) make PD0 out
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFF0FFFF); //GPIO
	GPIO_PORTD_AFSEL_R &= ~0xF0;      // 5) regular port function
  GPIO_PORTD_DEN_R |= 0x01;         // 6) enable digital I/O on PD0
}

void Sound_Stop(void){
	NVIC_ST_RELOAD_R = 0;// reload value
}

void Sound_Start(void){
	NVIC_ST_RELOAD_R = 79999;
}

void Clock_Init(void){
	hour = 0;
	minute = 0;
	second = 0;
	Sound_Init(); //PD0
	SysTick_Init(80000); //sound 1khz
	Timer0A_Init(&Update_Time, 79999500, 0); //period of ~1sec
}
