// Display.c
// Runs on LM4F120/TM4C123
// Display interface for alarm clock
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

void Display_Mode(uint8_t mode){

}

void Update_Display(uint8_t mode){

}

void Display_Init(void){
	ST7735_InitR(none);
	//Display analog mode first
}
