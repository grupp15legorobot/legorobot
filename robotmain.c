#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <string.h>
#include <ev3.h>
#include <ev3_port.h>
#include <ev3_sensor.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_RIGHT        OUTA   /*Vänster motor*/
#define MOTOR_LEFT        OUTB    /*Höger motor*/
#define MOTOR_C            OUTC    /*Lägg servomotor här*/
#define MOTOR_D            OUTD
#define SENSOR_TOUCH    IN1    /*Avståndssensor*/
#define SENSOR_US        IN2     
#define SENSOR_3        IN3
#define SENSOR_4        IN4
#define MOTOR_BOTH         ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */

int max_hastighet;         /* variabel för max hastighet på motorn */
int max_hastighet_c;
POOL_T touchSensor, us_sensor;  


/*Här börjar main koden*/
int main( void )
{  
    /* uint8_t sn_touch, sn_color, sn_US; */
    
    if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
    printf( "*** ( EV3 ) Hello! ***\n" );
    Sleep( 2000 );
    
    if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );    /* Kollar maxhastigheten som motorn kan ha */
        max_hastighet_c = tacho_get_max_speed(MOTOR_C,0);   

        tacho_reset( MOTOR_BOTH );
    } else {
        printf( "Anslut vänster motor i port A,\n"
        "Anslut höger motor i port B.\n"
        );
          brick_uninit();
        return ( 0 );  /* Stänger av sig om motorer ej är inkopplade */
    }
    
    
    //tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.5 );  // Sätter hastigheten på båda motorerna till 50% av maxhastigheten
    /* Om man vill köra bakåt anger man negativ hastighet, till exempel max_hastighet * (-0.5) */
    
    /*
    tacho_run_forever(  MOTOR_BOTH );
    tacho_stop( MOTOR_LEFT );
    tacho_stop( MOTOR_RIGHT );
    */
    
    printf("test1 \n");
    us_sensor=sensor_search( LEGO_EV3_US );
    us_set_mode_us_dist_cm(us_sensor);
    int distance;
    int tempdist;
    distance=sensor_get_value(0, us_sensor, 0); //Se nuvarande avstånd
    printf("test2 \n");
    
    tacho_set_speed_sp( MOTOR_LEFT, max_hastighet * -0.1 );
    tacho_run_forever(  MOTOR_LEFT ); //Snurra runt
    tacho_set_speed_sp( MOTOR_RIGHT, max_hastighet * 0.1 );
    tacho_run_forever(  MOTOR_RIGHT ); //Snurra runt

    while(1)  //while true
    {
    tempdist=sensor_get_value(0, us_sensor, 0);    //Tar avstånd
        
        if (tempdist<distance)  //Om avståndet minskar
        {
            distance=tempdist;      //Spara nya värdet som distance
            printf("%d \n", distance);
            Sleep(50);
        }    
        else if (tempdist>distance && tempdist<530 && tempdist>470) //Om avståndet ökar och inom intervall då stanna
        {
            break;
        }
    }
    tacho_stop(  MOTOR_BOTH ); //stanna motor
    Sleep(50);
    tacho_set_speed_sp( MOTOR_LEFT, max_hastighet * 0.1 );
    tacho_run_forever(MOTOR_BOTH);
    Sleep(15000);
    tacho_stop(MOTOR_BOTH);
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * -0.1 );
    tacho_run_forever(MOTOR_BOTH);
    Sleep(3100);
    tacho_stop(MOTOR_BOTH);
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.1 );
    tacho_run_forever(MOTOR_RIGHT);
    Sleep(3000);
    tacho_stop(MOTOR_RIGHT);
    Sleep(100);
    tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.3 );
    tacho_run_forever(MOTOR_BOTH);
    Sleep(20000);
    tacho_set_speed_sp( MOTOR_C, max_hastighet_c * -0.3 );
    tacho_run_forever(MOTOR_C);
    Sleep(1000);
    tacho_stop(MOTOR_C);

    brick_uninit();
    printf( "dying...\n" );
    return ( 0 );

}
