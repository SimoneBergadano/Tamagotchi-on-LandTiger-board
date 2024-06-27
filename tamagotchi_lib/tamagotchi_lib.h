
//flag stato di reset
extern char reset;

// Selezione del FoodMenu
extern char food_selection;
extern char eating;
extern char cuddling;
extern char remaining_notes;
extern int volume;

void Tamagotchi_Gui_Inizializzation(void);
void drawTamagotchi(void);
void drawDeadTamagotchi(void);
void drawStatusMenu(int hh, int mm, int ss, int Happiness, int Satiety);
void drawFoodMenu(void);
void drawResetMenu(void);
void swapFoodSelection(char selection);
void update_time(void);
void look_animation(void);
void incrementHappiness(int);
void incrementSatiety(int);
void resetTamagotchi(void);
void mouthAnimation(int);
void cuddlingFun(void);
void showVolume(int v);
void makeSound(int n);
void playNote(int note);
void stopSound(void);




