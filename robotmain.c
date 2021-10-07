#include <stdio.h> 
#include "brick.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */
#define MOTOR_RIGHT    OUTA
#define MOTOR_LEFT    OUTB
#define MOTOR_C    OUTC
#define MOTOR_D    OUTD
#define SENSOR_TOUCH IN1
#define SENSOR_2 IN2
#define SENSOR_3 IN3
#define SENSOR_4 IN4
#define SENSOR_US		IN4
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */
int max_hastighet;         /* variabel för max hastighet på motorn */ 
POOL_T touch_sensor;
POOL_T us_sensor;
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
  
  touch_sensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensorpå touchSensor-variabeln
  us_sensor = sensor_search( LEGO_EV3_US );
	us_set_mode_us_dist_cm(us_sensor);
	int distance;
	distance=sensor_get_value(0, us_sensor,0);
	printf("%d", distance);

  
  touch_set_mode_touch(touch_sensor); // anger vilken "mode" sensorn skall ha
  /* 
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
 
 */ 


  while(distance < 10) {
  printf("%d",distance);
     move_straight();
    Sleep( 4000 );
  }//Så länge touch-sensorn inte ärintryckt kommer while-loopen köras

  
  /*while(!sensor_get_value(0, touch_sensor, 0)); //Så länge touch-sensorn inte ärintryckt kommer while-loopen köras
  move_back();
    */    
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
