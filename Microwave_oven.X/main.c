/*
 * File:   main.c
 * Author: Amey Gunde
 *
 * Created on 14 March, 2025, 2:50 PM
 */

#include "main.h" 

#pragma config WDTE = OFF 

unsigned char operational_flag = POWER_ON_SCREEN; 
unsigned char reset_flag = RESET_NOTHING; 
extern unsigned char min, sec; 

void init_config(void) 
{
    // configure CLCD 
    init_clcd(); 
    
    // configure matrix keypad
    init_matrix_keypad(); 
    
    // configure FAN 
    FAN_DDR = 0; 
    FAN = 0;

    //configure BUZZER 
    BUZZER_DDR = 0; 
    BUZZER = 0; 
    
    //configure timer 2
    init_timer2(); 
    PEIE = 1; 
    GIE = 1; 
} 


void main(void) 
{ 
    unsigned char key;
    unsigned char start_mode = 0; 
    init_config(); 
    
    while (1) 
    {          
        key = read_matrix_keypad (STATE); //0 1 2 3 4 5 6 7 8 9 * # 
        
        // select the cooking mode based on key press, in menu screen
        if (operational_flag == MENU_SCREEN) 
        {
           if (key == 1) 
            { 
                clear_screen(); 
                operational_flag = MICRO_MODE; 
                start_mode = 0; 
                clcd_print("Power = 900W", LINE2(0)); 
                __delay_ms(2000); 
                clear_screen(); 
                reset_flag = RESET_MODE; 
            }
            else if (key == 2) 
            {  
                clear_screen(); 
                start_mode = 0; 
                operational_flag = GRILL_MODE; 
                reset_flag = RESET_MODE;
            } 
            else if (key == 3) 
            {
                clear_screen();
                start_mode = 0; 
                operational_flag = CONVECTION_MODE; 
                reset_flag = RESET_MODE;
            } 
            else if (key == 4) 
            { 
                clear_screen(); 
                operational_flag = START_MODE; 
                start_mode = 1; 
                reset_flag = RESET_MODE; 
            } 
        } 
        if (operational_flag == DISPLAY_TIME) 
        { 
            if (key == 4) 
            {       
                //incrèment cooking time by 30. sec
                if (start_mode) 
                { 
                    sec = sec + 30; 
                    if (sec > 59) 
                    { 
                        min++; 
                        sec = sec - 60; 
                    } 
                } 
            } 
            else if (key == 5) 
            {
                operational_flag = PAUSE;  
            } 
            else if (key == 6) 
            { 
                operational_flag = STOP; 
            } 
        }    
        else if (operational_flag == PAUSE)
        { 
            //resume the micro oven 
            if (key == 4) 
            {
                FAN = 1; 
                TMR2ON = 1; 
                operational_flag = DISPLAY_TIME; 
            }
        }
        
        
        switch (operational_flag)
        {
            case POWER_ON_SCREEN : 
                
                // display power on screen 
                power_on_screen(); 
                operational_flag = MENU_SCREEN;
                clear_screen(); 
                break; 

            case MENU_SCREEN :
                
                // display menu 
                display_menu_screen(); 
                break; 
                                
            case MICRO_MODE :
                
                // read the key based on key SET T??? 
                set_time (key, reset_flag); 
                break; 
                                
            case DISPLAY_TIME :
                
                // display time 
                time_display(); 
                break;
                                
            case PAUSE :
                
                // stop micro oven and also the timer 
                FAN = 0; 
                TMR2ON = 0;
                break; 
                            
            case STOP :
                
                // stop micro oven and also the timer 
                FAN = 0; 
                TMR2ON = 0; 
                clear_screen(); 
                operational_flag = MENU_SCREEN;
                break; 
                            
            case GRILL_MODE :
                
                // set time
                set_time(key, reset_flag); 
                break; 
                                
            case CONVECTION_MODE :
                
                // set temperature, set time, display time
                set_temp(key, reset_flag); 
                break; 
                                    
            case START_MODE :
                
                // Display time with 30sec as cooking time 
                heat_food(); 
                break; 
        } 
        reset_flag = RESET_NOTHING; 
    }
}