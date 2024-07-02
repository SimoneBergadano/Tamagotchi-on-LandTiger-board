/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

#include "../tamagotchi_lib/tamagotchi_lib.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	/* joystick select functionality */
  LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.29)
	LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.25 Input (joysticks on PORT1 defined as Input) 
}

void joystick_pool_LEFT(void) {
	
	static int up;
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick LEFT pressed */
		up++;
		switch(up){
			case 1:
					// Azione pressione joystick LEFT
					if(!eating && !reset) swapFoodSelection(0);
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
}

void joystick_pool_RIGHT(void) {
	
	static int up;
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick RIGHT pressed */
		up++;
		switch(up){
			case 1:
					// Azione pressione joystick RIGHT
					if(!eating && !reset) swapFoodSelection(1);
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
}

void joystick_pool_SEL(void) {
	
	static int up;
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick SEL pressed */
		up++;
		switch(up){
			case 1:
				// Azione pressione SEL joystick
					if(reset==1)resetTamagotchi();
					else if(food_selection==0 && !eating){
						incrementSatiety(1);
					}
					else if(food_selection==1 && !eating){
						incrementHappiness(1);
					}
					if(remaining_notes==0){
						remaining_notes=1;
						playNote(7);
					}
					
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
}


