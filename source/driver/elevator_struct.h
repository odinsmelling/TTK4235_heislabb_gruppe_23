#pragma once


typedef struct Elevator{
    int floor;
    int direction;
    int stopp_activated;
    int next_floor;
    int door_open;
    int is_order_list_empty;
    int testvarable;
    int last_floor;
    int last_direction;
    int active_order_handling;
    int obstruction_activated;
    int stop_recently_pressed;
    int recent_arrival;
    
} Elevator;