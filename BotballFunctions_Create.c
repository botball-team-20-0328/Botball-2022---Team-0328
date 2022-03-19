/*
Reference For Usefull Create Functions 
*/

#include <kipr/botball.h>

//Constants For One Arm (Port, Servo Positions For Where We Needed It To Go)
#define ARM_SERVO_PORT 0
#define ARM_RETRACTED 0
#define ARM_EXTENDED 1425
#define ARM_EXTENDED_HIGHER 1400
#define ARM_DOWN 2047
#define ARM_START 1814

//Constants For Other Arm (Port, Servo Positions For Where We Needed It To Go)
#define CLAW_ARM_SERVO_PORT 3
#define CLAW_ARM_RETRACTED 1900
#define CLAW_ARM_UP 1300
#define CLAW_ARM_EXTENDED 207
#define CLAW_ARM_EXTENDED_FURTHER 0
#define CLAW_ARM_FINISH 741

//Constants For Claw On Arm (Port, Servo Positions For Where We Needed It To Go)
#define CLAW_SERVO_PORT 2
#define CLAW_OPEN 900
#define CLAW_OPEN_WIDE 1900
#define CLAW_CLOSED 200

//Light Sensor Constants (Data Values To Indicate lines, Ports)
#define LEFT_LIGHT_SENSOR_PORT 5
#define LEFT_LIGHT_SENSOR_THRESH 3000 //200-3800
#define RIGHT_LIGHT_SENSOR_PORT 4
#define RIGHT_LIGHT_SENSOR_THRESH 3000 //200-3800

//Sensor We Will Use For Light Start
#define LIGHT_START_PORT 2

void create_measured_drive(int distance, int speedLeft, int speedRight);
void line_square_up(int speed);
void drive_until_bump(int speed);
void slow_servo(int servoPort, int currentPosition, int finishPosition);

int main(){

    //Need To Have For Competition
    create_connect();
    msleep(1000);
    //Light Start
   	wait_for_light(LIGHT_START_PORT); //put wait-for-light crap here
    shut_down_in(119);


    //Drive Create
    create_drive_direct(50, 50);
    msleep(5000);
    create_stop();

    //Line Square Up
    line_square_up(-25);

    ///Set Servo Positions
    set_servo_position(ARM_SERVO_PORT, ARM_EXTENDED);

    //Use Slow Servo Function
    slow_servo(CLAW_ARM_SERVO_PORT, CLAW_ARM_UP, CLAW_ARM_FINISH);

    //END
    create_stop();
    create_disconnect();
    disable_servos();
    return 0;
}

//Didn't End Up Using - Uses Sensors To Measure Create Distance, Not Really Needed
void create_measured_drive(int distance, int speedLeft, int speedRight) {
    set_create_distance(0);
    while (get_create_distance() < 1000)
    {
        create_drive_direct(200, 200);
    }
    create_stop();
}

void line_square_up(int speed) {
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
}

//Uses Squishy Bumb Sensor On Create
void drive_until_bump(int speed){
    while (get_create_lbump() == 0 || get_create_rbump() == 0)
    {
        create_drive_direct(speed, speed);
    }
    create_stop();
}

//Moves Servo...Slowly
void slow_servo(int servoPort, int currentPosition, int finishPosition)
{
    set_servo_position(servoPort, currentPosition);
    msleep(200);
    while (currentPosition != finishPosition)
    {
        if (currentPosition < finishPosition)
        {
            currentPosition += 1;
        }
        if (currentPosition > finishPosition)
        {
            currentPosition -= 1;
        }
        set_servo_position(servoPort, currentPosition);
        msleep(1);
    }
}
