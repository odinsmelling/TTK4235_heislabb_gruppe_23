#pragma once
#include "elevator_struct.h"
#include <time.h>


void cab_button_pressed(int cab_buttons[], int order_list[]);

void order_button_pressed(int up_buttons[], int down_button[],int cab_buttons[],int order_list[], Elevator *min_heis);

void stopped_pressed(Elevator *min_heis, int order_list[], int up_buttons[], int down_button[], int cab_buttons[], time_t *stop_time_stopp_button,time_t current_time);
