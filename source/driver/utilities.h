#pragma once

#include "elevator_struct.h"



void destinatin_floor(int order_list[], Elevator *min_heis);

void order_protocol(int order_list[], int up_buttons[], int down_button[], int cab_buttons[],  Elevator *min_heis);

void travel_to_destination_floor(Elevator *min_heis);

void arrival_at_destination_floor(Elevator *min_heis, int floor, time_t current_time, time_t *stop_time, int order_list[], int up_buttons[], int down_button[], int cab_buttons[] );

void motor_up_down(Elevator *min_heis);

void obstruction_activated(Elevator *min_heis, time_t current_time, time_t *stop_time_obstruction, int order_list[], int up_buttons[], int down_button[]);




