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
#include "elevio.h"
#include "light_protocol.h"



void cab_button_pressed(int cab_buttons[], int order_list[]){
    for(int j = 0; j < 4; j++){
        if (cab_buttons[j] == 1){
            order_list[j] = 1;
        }

    }
}

void order_button_pressed(int up_buttons[], int down_button[],int cab_buttons[], int order_list[], Elevator *min_heis){


    for ( int i = 0; i < 4; i++){
        if (elevio_callButton(i, 0)){
            if (!min_heis->recent_arrival){
                order_list[i] = 1;
            }
            
            if(min_heis->floor != i){
                up_buttons[i] = 1;
                
            }  
        }

    }

    
    for ( int j = 0; j < 4; j++){
        if (elevio_callButton(j, 1)){
            if (!min_heis->recent_arrival){
                order_list[j] = 1;
            }
            if(min_heis->floor != j){
                down_button[j] = 1;
            }
        }
    }


    for ( int k = 0; k < 4; k++){
        if (elevio_callButton(k, 2)){
            if (!min_heis->recent_arrival){
                order_list[k] = 1;
            }
            if(min_heis->floor != k){
                cab_buttons[k] = 1;
            }
        }
        }

    

    //printf("up button: %d," , up_buttons[1]);


}

void stopped_pressed(Elevator *min_heis, int order_list[], int up_buttons[], int down_button[], int cab_buttons[],time_t *stop_time_stopp_button,time_t current_time){
    if(elevio_stopButton()){
        
        if(min_heis->direction != 0){
            min_heis->last_direction = min_heis->direction;
        }

        min_heis->stopp_activated = 1;
        min_heis->stop_recently_pressed = 1;
        min_heis->direction = 0;
        elevio_motorDirection(DIRN_STOP);
        min_heis->next_floor = 5;
        stop_button_light(min_heis);
        for(int g = 0; g < 4; g++){
            down_button[g] = 0;
            cab_buttons[g] = 0;
            order_list[g] = 0;
            up_buttons[g] = 0;
        }

        order_button_lights(up_buttons, down_button, cab_buttons);

        //printf("floor: %d\n", min_heis->floor);
        if(min_heis->floor != -1){
            min_heis->door_open = 1;
            door_open_light(min_heis);
            *stop_time_stopp_button = current_time;
        }        
        else{
            
        }
    }

    if (!elevio_stopButton()){

        min_heis->stopp_activated = 0;
    }
    
   
    if (((current_time - *stop_time_stopp_button) >= 3) && (min_heis->stop_recently_pressed))
            {
                min_heis->door_open = 0;
                
                *stop_time_stopp_button = 0;
                
                
            }
}