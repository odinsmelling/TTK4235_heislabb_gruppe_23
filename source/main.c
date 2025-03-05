#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include <string.h>
#include <time.h>

#include "driver/utilities.h"
#include "driver/elevator_struct.h"
#include "driver/button_handling.h"
#include "driver/light_protocol.h"


int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    Elevator min_heis;
    int order_list[4] = {0,0,0,0};
    int up_buttons[4] = {0,0,0,0};
    int down_button[4] = {0,0,0,0};
    int cab_buttons[4] = {0,0,0,0};
    min_heis.stopp_activated = 0;
    min_heis.door_open = 0;
    min_heis.is_order_list_empty = 1;
    min_heis.next_floor = 5;
    min_heis.direction = 0;
    min_heis.last_direction = 0;
    min_heis.active_order_handling = 0;
    min_heis.obstruction_activated = 0;
    min_heis.stop_recently_pressed =0;
    min_heis.recent_arrival =0;
    elevio_motorDirection(DIRN_STOP);
    min_heis.testvarable =0;

    time_t stop_time = 0;
    time_t stop_time_obstruction = 0;
    time_t stop_time_stopp_button = 0;

    while(elevio_floorSensor() == -1){
        elevio_motorDirection(DIRN_DOWN);
    }

    elevio_motorDirection(DIRN_STOP);


    while(1){
        int floor = elevio_floorSensor();
        min_heis.floor = floor;
        //printf("Next_floor: %d, Last floor: %d , last_directon: %d  \n",min_heis.next_floor, min_heis.last_floor, min_heis.last_direction);
        time_t current_time = time(NULL);
        stopped_pressed(&min_heis, order_list, up_buttons, down_button, cab_buttons, &stop_time_stopp_button, current_time);
        arrival_at_destination_floor(&min_heis, floor, current_time, &stop_time, order_list, up_buttons, down_button, cab_buttons);



        
        //printf("  Order etter ordetprotocol: %d ", up_buttons[1]);
        order_button_pressed(up_buttons, down_button, cab_buttons, order_list, &min_heis);
        
      
        order_button_lights(up_buttons, down_button, cab_buttons);
       

        if (floor != -1){
            min_heis.last_floor = floor;
        }

        //printf("Next_floor %d\n",min_heis.next_floor);
        
        floor_incicator_light(min_heis.floor);
        
        stop_button_light(&min_heis);
        door_open_light(&min_heis);
        
        if(floor == 0){
            //elevio_motorDirection(DIRN_UP);
            //min_heis.direction = 1;
        }

        if(floor == N_FLOORS-1){
            //elevio_motorDirection(DIRN_DOWN);
            //min_heis.direction = -1;
        };
        cab_button_pressed(cab_buttons, order_list);
        
        destinatin_floor(order_list, &min_heis);
        
        
        //printf("  Order Main: %d \n", up_buttons[1]);

        order_protocol( order_list, up_buttons, down_button, cab_buttons, &min_heis);
        
        motor_up_down(&min_heis);
        
        obstruction_activated(&min_heis, current_time, &stop_time_obstruction, order_list,up_buttons, down_button);
        

        
        //if(elevio_stopButton()){
        //    elevio_motorDirection(DIRN_STOP);
        //    break;
        //}
        nanosleep(&(struct timespec){0, 20}, NULL);
        
    }

    return 0;
}
