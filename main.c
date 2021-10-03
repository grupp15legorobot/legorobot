#include <stdio.h> 
#include "brick.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */
#define MOTOR_RIGHT    OUTA
#define MOTOR_LEFT    OUTB
#define MOTOR_C    OUTC
#define MOTOR_D    OUTD
#define SENSOR_TOUCHIN1
#define SENSOR_2IN2
#define SENSOR_3IN3
#define SENSOR_4IN4
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */
int max_hastighet;         /* variabel för max hastighet på motorn */
POOL_T touchSensor;
POOL_T sonic_sensor;
void move_back();
void move_straight();
void move_right();
void move_left();  


int main( void )
{  
  if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
  printf( "*** ( EV3 ) Hello! ***\n" );
  Sleep( 2000 );
  
  if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */        
    max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );/* Kollar maxhastigheten som motorn kan ha */        
    tacho_reset( MOTOR_BOTH );    
  } 
  else {        
    printf( "Anslut vänster motor i port A,\n""Anslut höger motor i port B.\n");
    brick_uninit();        
    return (0);  /* Stänger av sig om motorer ej är inkopplade */    
  } 

	sonic_sensor = sensor_search(LEGO_EV3_US);
	us_set_mode_us_dist_cm(sonic_sensor);
	int us_distance;
	us_distance = (sensor_get_value(0, sonic_sensor, 0))/10; 
  
  touchSensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensorpå touchSensor-variabeln
  touch_set_mode_touch(touchSensor); // anger vilken "mode" sensorn skall ha
  move_straight();
  Sleep( 1000 );
        
  tacho_stop( MOTOR_BOTH );
  move_back();
  Sleep( 1000 );
  
  tacho_stop( MOTOR_BOTH );
  move_left();
    
  Sleep( 4000 );
    
  move_right();
        
  Sleep( 4000 );
        
  tacho_stop( MOTOR_RIGHT );
  brick_uninit();
  printf( "dying...\n" );    
        
  return (0);
}

void move_left(){
  tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.1 );
  tacho_run_forever( MOTOR_LEFT );
}

void move_right(){
  tacho_stop( MOTOR_LEFT );
  tacho_run_forever( MOTOR_RIGHT );
}

void move_straight(){
  tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.1 );// Sätter hastighetenpå båda motorerna till 50% av maxhastigheten/* Om man vill köra bakåt anger man negativ hastighet, till exempel max_hastighet * (-0.5) */
  tacho_run_forever( MOTOR_BOTH ); 
}

void move_back(){
  tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * -0.1 );
  tacho_run_forever( MOTOR_BOTH ); 
}


/* hittar var sensorn är inkopplad, ska vara innan koden */


/* ska vara i koden  kanske i en loop så det hela tiden uppdateras */