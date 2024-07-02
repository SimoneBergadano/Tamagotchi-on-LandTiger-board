/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "stdio.h"
#include "lpc17xx.h"
#include "timer.h"
#include "../joystick/joystick.h"
#include "../GLCD/GLCD.h"
#include "../adc/adc.h"

#include "../TouchPanel/TouchPanel.h"

#include "../tamagotchi_lib/tamagotchi_lib.h"

void poolTouchPannel(void);

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] =                                       /* ÕýÏÒ±í                       */
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	
	joystick_pool_LEFT();
	joystick_pool_RIGHT();
	joystick_pool_SEL();
	
	ADC_start_conversion();
	
	poolTouchPannel();
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	
	if(!reset){
		
		look_animation();
		update_time();
		
	};
	
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}


void poolTouchPannel(void){
	
  if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){
		if( display.x-(MAX_X/2)>-60 && display.x-(MAX_X/2)<60 && display.y-(MAX_Y/2)>-60 && display.y-(MAX_Y/2)<60 && !cuddling && !reset){
			cuddlingFun();
		}
		
	}
	
}


// -----------------------------------------------------------
// ----------------------- PARTE AUDIO -----------------------
// -----------------------------------------------------------


void TIMER2_IRQHandler (void)
{
	
	static int ticks=0;
	/* DAC management */	
	LPC_DAC->DACR = (SinTable[ticks]<<6)*volume;
	ticks++;
	if(ticks==45) ticks=0;
	
	
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	
	static int i=0;
	
	disable_timer(2);
	reset_timer(2);
	
	remaining_notes--;
	if(remaining_notes>0)playNote((++i)%8);
	
	
	
	
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}




/******************************************************************************
**                            End Of File
******************************************************************************/
