#pragma once
#include "elevator_struct.h"

void floor_incicator_light(int floor);

void order_button_lights(int up_buttons[], int down_button[],int cab_buttons[]);

void stop_button_light(Elevator *min_heis);

void door_open_light(Elevator *min_heis);