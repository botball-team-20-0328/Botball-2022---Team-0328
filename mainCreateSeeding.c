#include <kipr/wombat.h>
#include "roomba.h"

#define LIFT_PORT 3
#define LCLAW_PORT 2
#define RCLAW_PORT 3

#define LIFT_SENSOR_PORT 9

#define LIFT_LOW -11000
#define LIFT_MID -5000
#define LIFT_HIGH 0

#define LIFT_TOP -100
#define LIFT_ONETWO -6200
#define LIFT_THREE -7620
#define LIFT_FOUR -8870
#define LIFT_FIVE -9900

#define LIFT_DROP_H -2000
#define MOTOR_MARGIN 10

#define LCLAW_THRESH 310
#define RCLAW_THRESH 1931
#define CLAW_OPEN 0
#define CLAW_MID 318
#define CLAW_CLOSED 770

#define CLAW_ONETWO 710
#define CLAW_THREE 670
#define CLAW_FOUR 630
#define CLAW_FIVE 600

#define CREATE_LIGHT_THRESH 2000
#define CREATE_360_CONST 5350

#define LIGHT_START_PORT 5

double bias;

void arm_all();
void slow_servo(int port, int position, int time);
void lift(int position, int spd);
void resetLift();
void claw_pos(int position, int time);
void line_square_up(int speed);
void create_turn(int degree, int speed);

int main()
{
    arm_all();
    /*while(1) {
        printf("VAL: %d\n", get_create_rfcliff_amt());
        msleep(500);
    }*/
	//create_connect();
    //create_full();
    printf("BATTERY: %d\n", get_create_battery_capacity());

    //roomba_drive_straight(int distance, int speed)
    //speed determines direction, distance always positive
    //create_turn(int degrees, int speed)
    //degrees determines direction (CCW= +), speed always positive
    //roomba_turn_with_radius(int angle, int radius, int speed)
    //create_turn(int angle, int speed)

    ///TOURNAMENT FUNCTIONS
    wait_for_light(LIGHT_START_PORT);
    shut_down_in(119);
    msleep(4000);


    create_drive_direct(-100,-100);
    claw_pos(CLAW_OPEN,500);
    roomba_drive_straight(178,200);
    create_turn(128,100);
    roomba_drive_straight(55,200);
    //drop claw, pick up rings, drive to post
    lift(LIFT_ONETWO,1300);
    claw_pos(CLAW_ONETWO,1000);
    lift(LIFT_TOP,1300);
    
    //roomba_drive_straight(100,-200);
    create_drive_direct(-100,-100);
    msleep(600);
    create_turn(-128,100);
    
    //roomba_drive_straight(178,-300);
    //roomba_drive_straight(60,-150);
    create_drive_direct(-200,-200);
    msleep(800);
    create_drive_direct(-100,-100);
    msleep(1000);
    //squared up, drive
    roomba_drive_straight(50,200);
    create_turn(-90,100);
    line_square_up(100);
    
    //out of base
    roomba_drive_straight(589,300);
    create_drive_direct(75,75);
    while(get_create_rcliff_amt()>CREATE_LIGHT_THRESH) {
        msleep(10);
    }
    create_stop();
    //back up from line
	create_drive_direct(-100,-100);
    msleep(1000);

    //turn towards pvc
    create_turn(-64,100);
    create_drive_direct(-50,-50);
    msleep(1000);
    create_stop();
    msleep(500);
    //over rod, drop rings
    lift(LIFT_DROP_H,1200);
    
    claw_pos(CLAW_OPEN,200);
    create_drive_direct(-100,-100);
    msleep(500);
    create_turn(265,100);
    
    //turned around, drive back to base
    roomba_drive_straight(200,200);
    line_square_up(100);
    roomba_drive_straight(275,200);
    create_turn(-90,100);
    create_drive_direct(-100,-100);
    msleep(2000);
    //squared up back in base, repeat same process
    
    
    
    
    
    
    
    
    
    ///////////////AGAAAAIN
    roomba_drive_straight(178,200);
    create_turn(128,100);
    roomba_drive_straight(55,200);
    //drop claw, pick up rings, drive to post
    lift(LIFT_FOUR,1300);
    claw_pos(CLAW_FOUR,1000);
    lift(LIFT_TOP,1300);
    
    //roomba_drive_straight(100,-200);
    create_drive_direct(-100,-100);
    msleep(600);
    create_turn(-128,100);
    
    //roomba_drive_straight(178,-300);
    //roomba_drive_straight(60,-150);
    create_drive_direct(-200,-200);
    msleep(800);
    create_drive_direct(-100,-100);
    msleep(1000);
    //squared up, drive
    roomba_drive_straight(50,200);
    create_turn(-90,100);
    line_square_up(100);
    
    //out of base
    roomba_drive_straight(589,300);
    create_drive_direct(75,75);
    while(get_create_rcliff_amt()>CREATE_LIGHT_THRESH) {
        msleep(10);
    }
    create_stop();
    //back up from line
	create_drive_direct(-100,-100);
    msleep(1000);

    //turn towards pvc
    create_turn(-63,100);
    create_drive_direct(-50,-50);
    msleep(1100);
    create_stop();
    msleep(500);
    //over rod, drop rings
    lift(LIFT_DROP_H,1200);
    
    claw_pos(CLAW_OPEN,200);
    create_drive_direct(-100,-100);
    msleep(500);
    create_turn(265,100);
    
    //turned around, drive back to base
    roomba_drive_straight(200,200);
    line_square_up(100);
    roomba_drive_straight(275,200);
    create_turn(-90,100);
    create_drive_direct(-100,-100);
    msleep(2000);
    //squared up back in base, repeat same process
    
    
    
    
    
    
    ///////////////AGAAAAIN
    roomba_drive_straight(178,200);
    create_turn(128,100);
    roomba_drive_straight(55,200);
    //drop claw, pick up rings, drive to post
    lift(LIFT_FIVE,1300);
    claw_pos(CLAW_FIVE,1000);
    lift(LIFT_TOP,1300);
    
    //roomba_drive_straight(100,-200);
    create_drive_direct(-100,-100);
    msleep(600);
    create_turn(-128,100);
    
    //roomba_drive_straight(178,-300);
    //roomba_drive_straight(60,-150);
    create_drive_direct(-200,-200);
    msleep(800);
    create_drive_direct(-100,-100);
    msleep(1000);
    //squared up, drive
    roomba_drive_straight(50,200);
    create_turn(-90,100);
    line_square_up(100);
    
    //out of base
    roomba_drive_straight(589,300);
    create_drive_direct(75,75);
    while(get_create_rcliff_amt()>CREATE_LIGHT_THRESH) {
        msleep(10);
    }
    create_stop();
    //back up from line
	create_drive_direct(-100,-100);
    msleep(1000);

    //turn towards pvc
    create_turn(-65,100);
    create_drive_direct(-50,-50);
    msleep(1100);
    create_stop();
    msleep(500);
    //over rod, drop rings
    lift(LIFT_DROP_H,1200);
    
    claw_pos(CLAW_OPEN,200);
    
    return 0;
}

void arm_all()
{
    create_connect();
    create_full();
    claw_pos(CLAW_THREE,1);
    enable_servos();
    resetLift();
    //cg();
    printf("Successfully Armed All.\n");
}

void slow_servo(int port, int position, int time){
    int beginPos = get_servo_position(port);
    int increment = beginPos<position?1:-1;
    int currentPos;
    for(currentPos=beginPos;currentPos!=position;currentPos+=increment){
        set_servo_position(port, currentPos);
        msleep(time/((position-beginPos)*increment));
    }
}

void lift(int position, int spd) {
    int cPos = get_motor_position_counter(LIFT_PORT);
    while ((cPos-MOTOR_MARGIN)>position || (cPos+MOTOR_MARGIN)<position) {
        cPos = get_motor_position_counter(LIFT_PORT);
        if (cPos < position) {
            //printf("POSITIVE REQD. CPOS:%d\n", cPos);
			//printf("END POSITION:%d\n",position); 
            mav(LIFT_PORT, spd);
        }
        if (cPos > position) {
            //printf("NEGATIVE REQD. CPOS:%d\n", cPos);
            //printf("END POSITION:%d\n",position); 
            mav(LIFT_PORT, -spd);
        }
        msleep(10);
    }
    mav(LIFT_PORT,0);
    msleep(200);

    /*int beginPos = get_motor_position_counter(port);
    int increment = beginPos<position?1:-1;
    int currentPos;
    for(currentPos=beginPos;currentPos!=position;currentPos+=increment){
        printf("INCREMENT AMT: %d\n", increment);
        mav(port, (get_motor_position_counter(port)-currentPos)*100);
        msleep(time/((position-beginPos)*increment));
    }*/
}
void resetLift() {
    mav(LIFT_PORT,750);
    while(digital(LIFT_SENSOR_PORT)==0) {
        msleep(10);
    }
    mav(LIFT_PORT,0);
    clear_motor_position_counter(LIFT_PORT);
    //lift(LIFT_LOW, 1500);
}
void claw_pos(int position, int time) {
    //WIP-----------------------------------------------------------------
    /*int currentPos = get_servo_position(LCLAW_PORT)-LCLAW_THRESH;
    set_servo_position(LCLAW_PORT, currentPos);
    set_servo_position(RCLAW_PORT, 1024-currentPos+1024);
    while (currentPos != finishPosition)
    {
        if (currentPos < finishPosition)
        {
            currentPos += 1;
        }
        if (currentPos > finishPosition)
        {
            currentPos -= 1;
        }
        set_servo_position(LCLAW_PORT, currentPos);
        set_servo_position(RCLAW_PORT, 1024-currentPos+1024);
        msleep(1);
    }*/

    int beginPos = get_servo_position(LCLAW_PORT)-LCLAW_THRESH;
    int increment = beginPos<position?1:-1;
    int currentPos;
    for(currentPos=beginPos;currentPos!=position;currentPos+=increment){
        set_servo_position(LCLAW_PORT, currentPos+LCLAW_THRESH);
        set_servo_position(RCLAW_PORT, RCLAW_THRESH-currentPos);
        //printf("L: %d", get_servo_position(LCLAW_PORT));
        //printf(" - R: %d", get_servo_position(RCLAW_PORT));
        //printf(" - VALUE: %d\n", currentPos);
        msleep(time/((position-beginPos)*increment));
    }
}

/*void line_square_up(int speed) {
    while (analog(LEFT_LIGHT_SENSOR_PORT) < LEFT_LIGHT_SENSOR_THRESH && analog(RIGHT_LIGHT_SENSOR_PORT) < RIGHT_LIGHT_SENSOR_THRESH)
    {
        create_drive_direct(speed, speed);
    }
    create_stop();
    while (analog(LEFT_LIGHT_SENSOR_PORT) < LEFT_LIGHT_SENSOR_THRESH)
    {
        create_drive_direct(speed, 0);
    }
    create_stop();
    while (analog(RIGHT_LIGHT_SENSOR_PORT) < RIGHT_LIGHT_SENSOR_THRESH)
    {
        create_drive_direct(0, speed);
    }
    create_stop();
    while (analog(LEFT_LIGHT_SENSOR_PORT) > LEFT_LIGHT_SENSOR_THRESH)
    {
        create_drive_direct(-speed, 0);
    }
    create_stop();
    while (analog(RIGHT_LIGHT_SENSOR_PORT) > RIGHT_LIGHT_SENSOR_THRESH)
    {
        create_drive_direct(0, -speed);
    }
    create_stop();
}*/



void line_square_up(int speed) {
    while((get_create_lcliff_amt() > CREATE_LIGHT_THRESH) || (get_create_rcliff_amt() > CREATE_LIGHT_THRESH)) {
        if((get_create_lcliff_amt() > CREATE_LIGHT_THRESH) && (get_create_rcliff_amt() <= CREATE_LIGHT_THRESH)) {
            create_drive_direct(0.75*speed, -0.1*speed);
        } else if((get_create_lcliff_amt() <= CREATE_LIGHT_THRESH) && (get_create_rcliff_amt() > CREATE_LIGHT_THRESH)) {
            create_drive_direct(-0.1*speed,0.75*speed);
        } else {
            create_drive_direct(speed,speed);
        }
    }
    create_stop();
}

void create_turn(int degree, int speed) {
	speed = 100;
    if(degree>=0) {
        create_drive_direct(-speed,speed);
    } else {
        degree=-degree;
        create_drive_direct(speed,-speed);
    }
    float stuff = (CREATE_360_CONST / (360.0/degree)) * 100.0/speed;
    msleep(stuff);
    create_stop();
}
