/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "adc/adc.h"

#include "TouchPanel/TouchPanel.h"


#include "tamagotchi_lib/tamagotchi_lib.h"

// aggiungere SIMULATOR come define nella sezione C/C++ se si esegue in simulazione rimuoverlo se si esegue sulla scheda
#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	
	TP_Init();
	TouchPanel_Calibrate();
	
	Tamagotchi_Gui_Inizializzation();
	
	joystick_init();											/* Joystick Initialization            */
	
	init_timer(0, 0, 0, 3, 0x004C4B40 ); 						// polling
	enable_timer(0);
	
	init_timer(1, 0, 0, 3, 25000000 ); 						// 1 secondo servirà per l'animazione 0x017D7840
	enable_timer(1);
	
	ADC_init();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);

	// Audio
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	NVIC_SetPriority(TIMER2_IRQn, 0);
	NVIC_SetPriority(TIMER3_IRQn, 1);
	NVIC_SetPriority(TIMER2_IRQn, 2);
	NVIC_SetPriority(TIMER1_IRQn, 3);
	
	
  while (1)	
  {
		__ASM("wfi");
  }
}
