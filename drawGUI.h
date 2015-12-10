

#ifndef DRAW_GUI
#define DRAW_GUI

#include "Arduino.h"
#include "global.h"

void initLCD();
void update_status(int flag);
void updateStr(int val);
void read_touch_buttons();
void draw_main_screen() ;
void draw_sensor_list() ;
void draw_test_status_screen();
void get_armdisarm_settings_from_sd(int flag);
void draw_arm_disarm_screen();
void draw_log_screen() ;
void display_arrows();
void draw_keypad();
void drawHome();
void display_images(); 
void display_tick_cross(int loc, int flag);
void make_control_panel_interface();
void draw_add_new_sensor_screen();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Declare fonts
extern uint8_t BigFont[];
extern uint8_t SmallFont[];

extern unsigned int SafeHome1[0x2418];



#endif


