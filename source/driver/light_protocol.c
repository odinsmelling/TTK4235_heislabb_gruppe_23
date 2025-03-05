#include "button_handling.h"
#include "elevator_struct.h"
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#include "utilities.h"
#include "con_load.h"
#include "elevator_struct.h"
#include "button_handling.h"
#include "elevio.h"

void floor_incicator_light(int floor){
    if(floor>=0){
        elevio_floorIndicator(floor);
    }
}

void order_button_lights(int up_buttons[], int down_button[],int cab_buttons[]){

    for(int i = 0; i < 4; i++){
        elevio_buttonLamp(i, 0, up_buttons[i]);
    }

    for(int j = 0; j < 4; j++){
        elevio_buttonLamp(j, 1, down_button[j]);
    }
    for(int k = 0; k < 4; k++){
        elevio_buttonLamp(k, 2, cab_buttons[k]);
    }
}

void stop_button_light(Elevator *min_heis){
    elevio_stopLamp(min_heis->stopp_activated);
}

void door_open_light(Elevator *min_heis){
    elevio_doorOpenLamp(min_heis->door_open);
}

