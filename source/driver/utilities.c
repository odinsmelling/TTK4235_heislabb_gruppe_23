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
#include <time.h>

void destinatin_floor(int order_list[], Elevator *min_heis)
{ // Når det er noe i order_list, blir is_order_list_empty = 0
    int j = 0;
    for (int i = 0; i < 4; i++)
    {
        if (order_list[i] == 1)
        {
            j++;
        }
    }

    if (j == 0)
    {
        min_heis->is_order_list_empty = 1;
    }
    else
    {
        min_heis->is_order_list_empty = 0;
    }
}

void order_protocol(int order_list[], int up_buttons[], int down_button[], int cab_buttons[], Elevator *min_heis)
{
    
    if (min_heis->door_open == 0)
    {

        if (min_heis->floor == -1)
        {

            for (int a = 0; a < 4; a++)
            {

                if (up_buttons[a])
                {
                    order_list[a] = 1;
                }
                if (down_button[a])
                {
                    order_list[a] = 1;
                }
            }
        }



        if ((min_heis->direction == 0) && !(min_heis->is_order_list_empty))
        { // hvis heis står i ro

            int initial_order = 0;
            

            for (int index = 0; index < 4; index++)
            {
                if (order_list[index] == 1)
                {
                    
                    initial_order = index;
                    break;
                }
            }

            
            min_heis->next_floor = initial_order;
            
            



            if (min_heis-> stop_recently_pressed){

                if (min_heis-> next_floor == min_heis->last_floor){
                    if(min_heis->last_direction == 1){
                        min_heis->last_floor++;
                    }
                    else if (min_heis->last_direction == -1){
                        min_heis->last_floor--;
                    }
                }
                min_heis->stop_recently_pressed = 0;

            
            }

            if (min_heis->last_direction == 1)
            {

                for (int index2 = min_heis->last_floor; index2 < 4; index2++)
                {
                    if (order_list[index2] == 1)
                    {
                        initial_order = index2;
                        min_heis->next_floor = initial_order;
                        break;
                    }
                }
            }

            int diff = min_heis->last_floor - initial_order;
            if (diff > 0)
            {
                min_heis->direction = -1;
            }
            else if (diff < 0)
            {
                min_heis->direction = 1;
            }
        }

        else if (((min_heis->direction == 1) && !(min_heis->floor == min_heis->next_floor)))
        {
            for (int k = min_heis->last_floor + 1; k < 4; k++)
            {
                if ((order_list[k]) && ((up_buttons[k]) || (cab_buttons[k])))
                {
                    min_heis->next_floor = k;
                    break;
                }
            };
        }

        else if ((min_heis->direction == -1) && !(min_heis->floor == min_heis->next_floor))
        {

            for (int k = min_heis->last_floor - 1; k > -1; k--)
            {
                if ((order_list[k]) && ((down_button[k]) || (cab_buttons[k])))
                {
                    min_heis->next_floor = k;
                    break;
                }
            };
        }
    }
}

void travel_to_destination_floor(Elevator *min_heis)
{
    int differnce = min_heis->next_floor - min_heis->floor;

    if (differnce > 0)
    {
        min_heis->direction = 1;
    }

    else if (differnce < 0)
    {
        min_heis->direction = -1;
    }
}

void motor_up_down(Elevator *min_heis)
{
    if (min_heis->direction == 1)
    {
        elevio_motorDirection(DIRN_UP);
    }
    else if (min_heis->direction == -1)
    {

        elevio_motorDirection(DIRN_DOWN);
    }
    else if (min_heis->direction == 0)
    {
        elevio_motorDirection(DIRN_STOP);
    }
}

void arrival_at_destination_floor(Elevator *min_heis, int floor, time_t current_time, time_t *stop_time, int order_list[], int up_buttons[], int down_button[], int cab_buttons[])
{
    
    if ((min_heis->next_floor == floor) && (min_heis->door_open == 0) && (min_heis->next_floor < 4))
    {
        //printf("  Next_floor %d\n", min_heis->next_floor);
        //printf("Order: %d \n", order_list[1]);
        min_heis->door_open = 1;
        min_heis-> recent_arrival = 1;
        min_heis->active_order_handling = 1;
        *stop_time = current_time;

        order_list[floor] = 0;

        cab_buttons[floor] = 0;

        if (min_heis->direction == 1)
        {
            up_buttons[floor] = 0;
        }

        else if (min_heis->direction == -1)
        {
            down_button[floor] = 0;
        }

        else if (min_heis -> direction == 0){
            //printf("  Floor %d \n" , floor);
            up_buttons[floor] = 0;
            down_button[floor] = 0;
            order_list[floor] = 0;
            printf("Order etter: %d ", up_buttons[1]);

        }



        if (floor == 0)
        {
            up_buttons[0] = 0;
        }
        if (floor == 3)
        {
            down_button[3] = 0;
        }

        

        min_heis->last_direction = min_heis->direction;
        min_heis->direction = 0;
    }
    if ((min_heis->next_floor == floor) && (min_heis->door_open == 1) && !(min_heis->obstruction_activated))
    {

        if (((current_time - *stop_time) >= 3))
        {
            min_heis->next_floor = 6;
            min_heis->door_open = 0;
            min_heis->active_order_handling = 0;
            *stop_time = 0;
            min_heis->recent_arrival = 0;
        }

        int j = 0;
        for (int i = 0; i < 4; i++)
        {
            if (order_list[i] == 1)
            {
                j++;
            }
        }

        if (j == 0)
        {
            min_heis->is_order_list_empty = 1;
        }
        else
        {
            min_heis->is_order_list_empty = 0;
        }

        if (min_heis->is_order_list_empty)
        {
            min_heis->last_direction = 0;
            for (int a = 0; a < 4; a++)
            {
                up_buttons[a] = 0;
                down_button[a] = 0;
                order_list[a] = 0;
            }

        }
    }
}

void obstruction_activated(Elevator *min_heis, time_t current_time, time_t *stop_time_obstruction, int order_list[], int up_buttons[], int down_button[])
{
    if ((min_heis->door_open) && (elevio_obstruction()) && (min_heis->next_floor != 5))
    {
        order_list[min_heis->floor] = 0;

        if (min_heis->last_direction == 1)
        {
            up_buttons[min_heis->floor] = 0;
        }

        else if (min_heis->last_direction == -1)
        {
            down_button[min_heis->floor] = 0;
        }
        else if (min_heis->last_direction == 0){
            up_buttons[min_heis->floor] = 0;
            down_button[min_heis->floor] = 0;
            order_list[min_heis->floor] = 0;

        }



        min_heis->direction = 0;
        //printf("open door %d\n", min_heis->door_open);
        min_heis->door_open = 1;
        *stop_time_obstruction = current_time;
        min_heis->obstruction_activated = 1;
    }
    if (((current_time - *stop_time_obstruction) >= 3) && (min_heis->obstruction_activated))
    {
        min_heis->door_open = 0;
        min_heis->obstruction_activated = 0;
        *stop_time_obstruction = 0;
        min_heis->next_floor = 7;
        min_heis->recent_arrival = 0;
        


    }
}