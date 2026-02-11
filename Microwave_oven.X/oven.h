/* 
 * File:   oven.h
 * Author: Amey gunde
 *
 * Created on 14 March, 2025, 3:13 PM
 */

#ifndef OVEN_H
#define	OVEN_H

void power_on_screen(void);
void clear_screen(void);
void display_menu_screen(void);
void set_time(unsigned char key, unsigned char reset_flag);
void set_temp(unsigned char key, unsigned char reset_flag);
void heat_food(void);

void time_display(void);

#endif	/* OVEN_H */