
#include "tamagotchi_lib.h"

#include "../GLCD/GLCD.h" 
#include "../timer/timer.h"
#include "../joystick/joystick.h"

#include "stdio.h"
#include "speaker_icon.h"

//flag stato
char reset=0;
char eating=0;
char cuddling=0;

char remaining_notes=0;

// Variabili tempo
int h = 0;
int m = 0;
int s = 0;

// Happiness/Satiety countdown
char countdown = 5;

// Variabili Felicità e soddisfazione
int Happiness = 5;
int Satiety = 5;

// Selezione del FoodMenu
char food_selection=0;

// Gestione suoni
int volume=3;

// Variabili Gui
int x0 = MAX_X/2;
int y0 = MAX_Y/2;
int distanza_occhi = 20;
int altezza_occhi = 15;
int x_bocca = 30;
int y_bocca = 25;

void drawTamagotchi(){
	
	int i, j, r;
	
	LCD_DrawCircle(x0, y0, 60, Black); // contorno
	
	LCD_DrawCircle(x0-distanza_occhi, y0-altezza_occhi, 15, Black); //occhio sinistro
	LCD_DrawCircle(x0+distanza_occhi, y0-altezza_occhi, 15, Black); //occhio destro
	LCD_DrawFullCircle(x0-distanza_occhi+5, y0-altezza_occhi+5, 15-7, Black); //pupilla sinistra
	LCD_DrawFullCircle(x0+distanza_occhi+5, y0-altezza_occhi+5, 15-7, Black); //pupilla destra
	
	
	//LCD_DrawLine(x0-x_bocca, y0+y_bocca, x0+x_bocca, y0+y_bocca, Black); //bocca
	r = 60;
	for (i = -r; i < +r; i++) {
		for (j = 45; j < +r; j++) {
			if( (i*i)+(j*j) <= (r*r) && j>=45) LCD_SetPoint(i+x0, j+y0-20, Black);
		}
	}
	
	showVolume(3);
	
}

void mouthAnimation(int state){
	int i, j, r;
	
	uint16_t color = Black;
	uint16_t color_txt = White;
	
	if (state==1)color = White; //cancello la vecchia bocca
	if (state==1)color_txt = Black; //cancello la vecchia bocca
	
	GUI_Text(MAX_X/2 - 50, y0+60+20, (uint8_t *) "I'm eating...", color_txt, White);
	
	r = 60;
	for (i = -r; i < +r; i++) {
		for (j = 45; j < +r; j++) {
			if( (i*i)+(j*j) <= (r*r) && j>=45) LCD_SetPoint(i+x0, j+y0-20, color);
		}
	}
	
	if(state==1){
			// bocca chiusa
			LCD_DrawLine(x0-x_bocca/2, y0+y_bocca, x0+x_bocca/2, y0+y_bocca, Black);
			LCD_DrawLine(x0-x_bocca/2, y0+y_bocca+1, x0+x_bocca/2, y0+y_bocca+1, Black);
	}
	
}




void cuddlingAnimation(int state){
	int i, j, r;
	
	uint16_t color1 = Black;
	uint16_t color2 = White;
	uint16_t color_txt = White;
	
	if (state==1){
		color1 = White; //cancello la vecchia bocca
		color2 = Black; //coloro la nuova
	}
	if (state==1)color_txt = Black; //cancello la vecchia bocca
	
	GUI_Text(MAX_X/2 - 50, y0+60+20, (uint8_t *) "ahahah", color_txt, White);
	
	r = 60;
	for (i = -r; i < +r; i++) {
		for (j = 45; j < +r; j++) {
			if( (i*i)+(j*j) <= (r*r) && j>=45) LCD_SetPoint(i+x0, j+y0-20, color1);
		}
	}
	
	
  // bocca sorridente
	r=60;
	for (i = -r; i < +r; i++) {
		 for (j = 45; j < +r; j++) {
				if( (i*i)+(j*j) - (r*r) <= 250  && (i*i)+(j*j) - (r*r) >= -200 && j>=45) LCD_SetPoint(i+x0, j+y0-20, color2);
	}
	}
	
	
}

void drawDeadTamagotchi(){
	int i, j, r;
	
	LCD_DrawFullCircle(x0, y0, 60, White); // ripulisco l'area
	LCD_DrawCircle(x0, y0, 60, Black); // contorno
	
	// occhi
	r=10;
	for (i = -r; i < +r; i++) {
		for (j = -r; j < +r; j++) {
			if( ( i-j<2 && i-j>-2) || ( i+j<2 && i+j>-2)){
				LCD_SetPoint(x0-distanza_occhi+i, y0-altezza_occhi+j, Black);
				LCD_SetPoint(x0+distanza_occhi+i, y0-altezza_occhi+j, Black);
			}
		}
	}
	// bocca
	LCD_DrawLine(x0-x_bocca/2, y0+y_bocca, x0+x_bocca/2, y0+y_bocca, Black);
	LCD_DrawLine(x0-x_bocca/2, y0+y_bocca-1, x0+x_bocca/2, y0+y_bocca-1, Black);

}

void drawStatusMenu(int hh, int mm, int ss, int Happiness, int Satiety){
	
	char age_string[20] = "";
	uint16_t color;
	
	
	sprintf(age_string, "AGE: %02d:%02d:%02d", hh, mm, ss);
	GUI_Text(70, 10, (uint8_t *)age_string, Black, White);
	
	GUI_Text(30, 30, (uint8_t *)"Happiness", Black, White);
	GUI_Text(30+MAX_X/2, 30, (uint8_t *)"Satiety", Black, White);
	
	LCD_DrawEmptyRectangle (30, 50, 57, 20, Black);
	LCD_DrawRectangle(30+57, 50+6, 6, 8, Black);
	LCD_DrawEmptyRectangle (30+MAX_X/2, 50, 57, 20, Black);
	LCD_DrawRectangle(30+57+MAX_X/2, 50+6, 6, 8, Black);
	
	color = Black;
	if(Happiness<1)color = White;
	if(Happiness==1)color = Red;
	LCD_DrawRectangle(32, 50+2, 10, 17, color);
	if(Happiness<2)color = White;
	LCD_DrawRectangle(43, 50+2, 10, 17, color);
	if(Happiness<3)color = White;
	LCD_DrawRectangle(54, 50+2, 10, 17, color);
	if(Happiness<4)color = White;
	LCD_DrawRectangle(65, 50+2, 10, 17, color);
	if(Happiness<5)color = White;
	LCD_DrawRectangle(76, 50+2, 10, 17, color);
	
	color = Black;
	if(Satiety<1)color = White;
	if(Satiety==1)color = Red;
	LCD_DrawRectangle(32+MAX_X/2, 50+2, 10, 17, color);
	if(Satiety<2)color = White;
	LCD_DrawRectangle(43+MAX_X/2, 50+2, 10, 17, color);
	if(Satiety<3)color = White;
	LCD_DrawRectangle(54+MAX_X/2, 50+2, 10, 17, color);
	if(Satiety<4)color = White;
	LCD_DrawRectangle(65+MAX_X/2, 50+2, 10, 17, color);
	if(Satiety<5)color = White;
	LCD_DrawRectangle(76+MAX_X/2, 50+2, 10, 17, color);
	
}

void drawFoodMenu(){
	
	LCD_DrawLine(0, MAX_Y-50, MAX_X, MAX_Y-50, Black);
	LCD_DrawLine(MAX_X/2, MAX_Y-50, MAX_X/2, MAX_Y, Black);
	
	GUI_Text(40, MAX_Y-30, (uint8_t *) "MEAL", Black, White); // Selezionato
	GUI_Text(40+MAX_X/2, MAX_Y-30, (uint8_t *) "SNACK", Black, White);
	
}

void swapFoodSelection(char selection){
	if (selection>1) return;
	food_selection = selection;
	if(food_selection==1){
		GUI_Text(40, MAX_Y-30, (uint8_t *) "MEAL", Black, White); 
		GUI_Text(40+MAX_X/2, MAX_Y-30, (uint8_t *) "SNACK", Red, White); // Selezionato
	}else{
		GUI_Text(40, MAX_Y-30, (uint8_t *) "MEAL", Red, White); // Selezionato
		GUI_Text(40+MAX_X/2, MAX_Y-30, (uint8_t *) "SNACK", Black, White);
	}
}

void drawResetMenu(){
	LCD_DrawRectangle (0, MAX_Y-50, MAX_X, 50, Red);
	GUI_Text(MAX_X/2 - 20, MAX_Y-30, (uint8_t *) "RESET", Black, Red);
}

void showVolume(int vol){
	
	int w, h;
	char *p = volume_icon;
	uint16_t pixel_dim=2;
	
	for(h=0; h<height; h++){
			for(w=0; w<width; w++){
				uint16_t color = HEADER_PIXEL(p);
				if(w>=8 && vol<1) color = White;
				if(w>=10 && vol<2) color = White;
				if(w>=13 && vol<3) color = White;
				LCD_DrawRectangle(w*pixel_dim+15, h*pixel_dim+100, pixel_dim, pixel_dim, color);
				p+=4;
			}
	}
	
}



void update_time(void){
	
	// Happiness/Satiety countdown
	static char countdown = 5;
	
	if(reset)return;
	
	if(eating){
		
		if(eating==1){
			remaining_notes=3;
			playNote(3);
		}
		
		eating--; //eating viene messa a 2 dopo 2 decrementi arriva a 0 (false)
		if(eating==0){
			mouthAnimation(0);
			swapFoodSelection(food_selection);
		}
	}
	
	if(cuddling){
		
		if(cuddling==4 && remaining_notes==0){
			remaining_notes=8;
			playNote(1);
			}
		
		cuddling--;
		cuddlingAnimation(cuddling%2);
		if(cuddling==0){
			//cuddlingAnimation(0);
			if(Happiness<5)Happiness++;
			drawStatusMenu(h, m, s, Happiness, Satiety); //per visualizzare subito il cambiamento
		}
	}
	
	// Aggiornamento tempo
	s++;
	if(s==60){
		m++;
		s=0;
		if(m==60){
			h++;
			m=0;
		}
	}
	countdown--;
	if(countdown==0){
		countdown = 5;
		Happiness--;
		Satiety--;
		if(Happiness==0 || Satiety==0){
			reset=1;
			disable_timer(1);
			LCD_Clear(White);
			drawStatusMenu(h, m, s, Happiness, Satiety);
			
			if(remaining_notes==0){
			remaining_notes=5;
			playNote(1);
			}
			
			drawDeadTamagotchi();
			drawResetMenu();
			return;
		}
	}
	drawStatusMenu(h, m, s, Happiness, Satiety);
}

void look_animation(void){
	
	// Animazione sguardo
	static char sguardo = 0;

	if(sguardo){
		LCD_DrawFullCircle(x0-distanza_occhi+5, y0-altezza_occhi+5, 15-7, White); //pupilla sinistra
		LCD_DrawFullCircle(x0+distanza_occhi+5, y0-altezza_occhi+5, 15-7, White); //pupilla destra
		
		LCD_DrawFullCircle(x0-distanza_occhi-5, y0-altezza_occhi+5, 15-7, Black); //pupilla sinistra
		LCD_DrawFullCircle(x0+distanza_occhi-5, y0-altezza_occhi+5, 15-7, Black); //pupilla destra
	}else{
		LCD_DrawFullCircle(x0-distanza_occhi-5, y0-altezza_occhi+5, 15-7, White); //pupilla sinistra
		LCD_DrawFullCircle(x0+distanza_occhi-5, y0-altezza_occhi+5, 15-7, White); //pupilla destra
		
		LCD_DrawFullCircle(x0-distanza_occhi+5, y0-altezza_occhi+5, 15-7, Black); //pupilla sinistra
		LCD_DrawFullCircle(x0+distanza_occhi+5, y0-altezza_occhi+5, 15-7, Black); //pupilla destra
	}
	sguardo = !sguardo;
	
}


void Tamagotchi_Gui_Inizializzation(void){
	
	LCD_Clear(White);
	
	drawStatusMenu(h, m, s, Happiness, Satiety);
	drawTamagotchi();
	drawFoodMenu();
	
	swapFoodSelection(food_selection);
	
}


void incrementHappiness(int n){
	if(Happiness<5) Happiness+=n;
	drawStatusMenu(h, m, s, Happiness, Satiety); //per visualizzare subilo il cambiamento
	eating = 2;
	mouthAnimation(1);
	drawFoodMenu();
}

void incrementSatiety(int n){
	if(Satiety<5)Satiety+=n;
	drawStatusMenu(h, m, s, Happiness, Satiety); //per visualizzare subilo il cambiamento
	eating = 2;
	mouthAnimation(1);
	drawFoodMenu();
}

void cuddlingFun(void){
	
	cuddling = 5;
	cuddlingAnimation(1);
	
}

void resetTamagotchi(void){
	
	reset=0;
	eating=0;
	h = 0;
	m = 0;
	s = 0;
	countdown = 5;
	Happiness = 5; 
	Satiety = 5;
	food_selection=0;
	
	remaining_notes=0;
	disable_timer(2);
	reset_timer(2);
	disable_timer(3);
	reset_timer(3);
	
	Tamagotchi_Gui_Inizializzation();
	
	enable_timer(1);
	
}

/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/

void playNote(int note){
	
	const int freqs[8]={2120,1890,1684,1592,1417,1263,1125,1062};
	
	if( note >= 8)return;
	
	disable_timer(2);
	reset_timer(2);
	init_timer(2, 0, 0, 3, freqs[note]);
	enable_timer(2);
	
	disable_timer(3);
	reset_timer(3);
	init_timer(3, 0, 0, 1, 12500000); // mezzo secondo tra una nota e l'altra
	enable_timer(3);
	
	NVIC_SetPriority(TIMER2_IRQn, 0);
	NVIC_SetPriority(TIMER3_IRQn, 1);
	NVIC_SetPriority(TIMER2_IRQn, 2);
	NVIC_SetPriority(TIMER1_IRQn, 3);
	
}

