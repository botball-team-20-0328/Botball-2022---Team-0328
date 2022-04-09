//wombat seeding
//gets 125 +- 4 points consistently
//10% chance of 30 +-4
#include <kipr/wombat.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//define ports
#define right_motor 1
#define left_motor 3
#define right_tophat 0
#define left_tophat 2
#define light_sensor 5
#define claw_min_height 1000
#define claw_max_height 2029
#define claw_servo 0
#define arm_servo 2
#define CLAW_BLOCK_CLOSED 460
#define CLAW_OPEN 1350
#define CLAW_AIRLOCK_CLOSED 445

#define ARM_UP 2047
#define ARM_PIPE_BLOCK 1225
#define ARM_DOWN 1111
#define ARM_MIN 
#define ARM_AIRLOCK 1697
//comment this out to turn off display_printf logging for some things
#define DEBUG 
void slow_servo(int port, int position, int time){
    int beginPos = get_servo_position(port);
    int increment = beginPos<position?1:-1;
    //because we can only wait integer ms at a time, keep track of rounding error
    //add it to ms.
    float rounding_error = 0;
    float time_increment = (float)time/((position-beginPos)*increment);
    int currentPos;
    for(currentPos=beginPos;currentPos!=position;currentPos+=increment){
        set_servo_position(port, currentPos);

        rounding_error+= time_increment - (int)time_increment;
        if((int)rounding_error>0){
            msleep(time_increment+rounding_error);
            rounding_error -= (int)rounding_error;
        }
        else{
            msleep(time_increment);
        }

    }
}

void move_motor_distance(int port, int spd, int distance) {
    const int error_margin = 5; //ticks of error that are acceptable
    int cPos = get_motor_position_counter(port);
    int desiredPos = cPos+distance;
    while ((cPos-error_margin)>desiredPos || (cPos+error_margin)<desiredPos) {
        cPos = get_motor_position_counter(port);
        if (cPos < desiredPos) {
            mav(port, spd);
        }
        if (cPos > desiredPos) {
            mav(port, -spd);
        }
        msleep(1);
    }
    mav(port,0);
}

struct ts {
    int right_white;
    int right_black;
    int right_thresh;
    int left_white;
    int left_black;
    int left_thresh;
};

struct ts tophat_settings = {
    217,
    3645,
    1931,
    395,
    3645,
    2020
};

struct ts calibrate_tophats(){
    struct ts output;
    display_clear();
    //get white values
    display_printf(0,0,"Place both tophats over white");
    display_printf(0,1,"Press A when finished");
    while(!a_button_clicked()){
        display_printf(0,2,"right value: %d    ", analog(right_tophat));
        display_printf(0,3,"left value: %d    ", analog(left_tophat));
        msleep(5);
    }
    output.right_white = analog(right_tophat);
    output.left_white = analog(left_tophat);
    msleep(1000);
    //get black values
    display_printf(0,4,"Place both tophats over black  ");
    display_printf(0,5,"Press A when finished ");
    while(!a_button_clicked()){
        display_printf(0,6,"right value: %d    ", analog(right_tophat));
        display_printf(0,7,"left value: %d    ", analog(left_tophat));
        msleep(5);
    }
    output.right_black = analog(right_tophat);
    output.left_black = analog(left_tophat);
    output.right_thresh = (output.right_black+output.right_white)/2;
    output.left_thresh = (output.left_black+output.left_white)/2;
    return output;
}

void tank_drive(int left, int right){
    mav(right_motor, right);
    mav(left_motor, left*0.94);//correct for slow right motor
}
//uses thread_params 0-5
void drive_distance(int distance, int speed){
    //0.96 is to correct for motor differences
    const int error_margin = 5; //ticks of error that are acceptable

    int leftPos = get_motor_position_counter(left_motor);
    int desiredPosLeft = leftPos+distance;

    int rightPos = get_motor_position_counter(right_motor);
    int desiredPosRight = rightPos+distance/0.94;
    bool leftDone = false;
    bool rightDone = false;
    while (!(leftDone&&rightDone)) {
        leftPos = get_motor_position_counter(left_motor);
        rightPos = get_motor_position_counter(right_motor);
        /*printf("\nleftPos: %d\n", leftPos);
        printf("rightPos: %d\n", rightPos);
        printf("desiredPosLeft: %d\n", desiredPosLeft);
        printf("desiredPosRight: %d\n", desiredPosRight);*/

        if (leftPos+error_margin < desiredPosLeft && distance > 0) {
            mav(left_motor, speed*0.94);//correct for motor diffs
            //printf("Left forwards\n");
        }
        else if (leftPos-error_margin> desiredPosLeft && distance<0) {
            mav(left_motor, -speed*0.94);
            //printf("Left back\n");
        }
        else{
            mav(left_motor, 0);
            leftDone = true;
            printf("LDone\n");
        }

        if (rightPos +error_margin < desiredPosRight&&distance>0) {
            mav(right_motor, speed);
            //printf("Right forwards\n");
        }
        else if (rightPos -error_margin > desiredPosRight&&distance<0) {
            mav(right_motor, -speed);
            //printf("Right back\n");
        }
        else{
			mav(right_motor, 0);
            rightDone = true;
            printf("RDone\n");
        }
        msleep(10);
    }
    tank_drive(0,0);

}
//only use this if you need different distances. Use drive_distance if they're the same
//this doesn't correct for motor differences
void tank_drive_distance(int left_dist, int right_dist, int speed){
    int left_speed = (left_dist>=right_dist)?speed:(left_dist*speed/right_dist);
    int right_speed = (left_dist<=right_dist)?speed:(right_dist*speed/left_dist);

    const int error_margin = 5; //ticks of error that are acceptable

    int leftPos = get_motor_position_counter(left_motor);
    int desiredPosLeft = leftPos+left_dist;

    int rightPos = get_motor_position_counter(right_motor);
    int desiredPosRight = rightPos+right_dist;
	printf("rightPos%d\n", rightPos);
    printf("desiredRight%d\n", desiredPosRight);
    bool leftDone = false;
    bool rightDone = false;
    while (!(leftDone&&rightDone)) {
        leftPos = get_motor_position_counter(left_motor);
        rightPos = get_motor_position_counter(right_motor);
        if (leftPos+error_margin < desiredPosLeft && left_dist > 0) {
            mav(left_motor, left_speed);//correct for motor diffs
            //printf("Left forwards\n");
        }
        else if (leftPos-error_margin> desiredPosLeft && left_dist<0) {
            mav(left_motor, left_speed);
            //printf("Left back\n");
        }
        else{
            mav(left_motor, 0);
            leftDone = true;
        }

        if (rightPos +error_margin < desiredPosRight&&right_dist>0) {
            mav(right_motor, right_speed);
            //printf("Right forwards\n");
        }
        else if (rightPos -error_margin > desiredPosRight&&right_dist<0) {
            mav(right_motor, right_speed);
            //printf("Right back\n");
        }
        else{
			mav(right_motor, 0);
            rightDone = true;
        }
    }
    tank_drive(0,0);
}

void line_square_up(int maxSpeed){
    //slopes for linear function between light and speed

    float right_slope = 1.2f*maxSpeed/(tophat_settings.right_white-tophat_settings.right_black);
    float left_slope = 1.2f*maxSpeed/(tophat_settings.left_white-tophat_settings.left_black);

    int right, left;
    float right_speed;
    float left_speed;

    while(analog(right_tophat)<tophat_settings.right_thresh&&analog(left_tophat)<tophat_settings.left_thresh){	
        tank_drive(maxSpeed, maxSpeed);
        msleep(5);
    }
	double start_time = seconds();
    const int max_time = 5;
    do{
        if(seconds()-start_time>max_time) break;
        right = analog(right_tophat);
        left = analog(left_tophat);
        right_speed = right_slope * (right-tophat_settings.right_white) + maxSpeed;
        left_speed = left_slope*(left-tophat_settings.left_white) + maxSpeed;
        tank_drive(left_speed, right_speed);
        msleep(10);
    }while(
        right_speed>7||right_speed<-7
        ||left_speed>7||left_speed<-7
    );
    ao();
}

void to_other_side() { //get it to the other side, poised to grab block
    enable_servos();
    slow_servo(2, 1155, 1000);
    printf("Hi, I'm Jensi! I'm giving you time to put me in the starting box after calibrating my tophats:)\n");
    printf("Press A button to start");
    while(!a_button_clicked()){
        msleep(5);
    }
    msleep(1000);
    tank_drive(-890,-890);
    msleep(4000);
    printf("pom done \n");
    line_square_up(500);
    printf("square done \n");
    slow_servo(2, 1500,1000);
    tank_drive(-800,-600);
    msleep(500);
    printf("first part of turn\n");
    tank_drive(105,900);
    msleep(2500);
    printf("driving to middle\n");
    msleep(500);
    line_square_up(600);//middle line
    msleep(100);
    printf("square up at middle\n");
    tank_drive(910,870);
    msleep(4600);
    printf("turn\n");
    slow_servo(2, 2047, 3000);
    msleep(5);
    tank_drive(5, 800);
    msleep(2400);

}
void block_up() {
    printf("switched functions to block_up!\n");
    enable_servos();
    line_square_up(-500);
    msleep(500);
    slow_servo(2, ARM_PIPE_BLOCK,2000);
    msleep(100);
    slow_servo(0, CLAW_OPEN,1000);
    msleep(500);
    printf("servos in position to grab\n");
    tank_drive(900,900);
    msleep(3400);
    set_servo_position(0, CLAW_BLOCK_CLOSED);
    msleep(100);
    printf("I grabbed the block! Are you proud?\n");
    line_square_up(-600);
    msleep(500);
    printf("squared up after block\n");
    tank_drive(-400,-900);
    msleep(2000);
    slow_servo(2, ARM_UP, 2300);
    msleep(500);
    printf("arm up\n");
    tank_drive(600,600);
    msleep(2100);
    printf("going to airlock\n");
    set_servo_position(0, CLAW_OPEN);
    printf("You had no faith in me Nikita how dare you :(. But look I did it!\n");
    msleep(500);
    tank_drive(-700,-700);
    msleep(3000);
}

void rip_dex() {
    enable_servos();
    tank_drive(700,200);
    msleep(3000);
    line_square_up(500);
    tank_drive(500, 700);
    msleep(5000);
    slow_servo(2, ARM_PIPE_BLOCK, 2000);
    msleep(500);
    tank_drive(600,600);
    msleep(1000);
    set_servo_position(0, CLAW_BLOCK_CLOSED);
    msleep(100);
    tank_drive(-600,-600);
    msleep(2000);
    line_square_up(-500);
    msleep(100);
}

const int default_speed = 1200;
//gets red block in starting box && goes to centerline
void to_centerline(){
    //exit starting box at hyperspeed
    /*line_square_up(-default_speed);
    drive_distance(-1200, default_speed);*/
    drive_distance(-4000, 1500);
    msleep(15000);
    line_square_up(default_speed);
    drive_distance(-800, default_speed);
    printf("A\n");
    set_servo_position(claw_servo, CLAW_OPEN);
    //turn and grab block
    
    tank_drive_distance(300, 1800, default_speed);
    printf("B\n");
    slow_servo(arm_servo, ARM_DOWN, 500);
    msleep(300);
    drive_distance(560, default_speed);
    set_servo_position(claw_servo, CLAW_BLOCK_CLOSED);
    msleep(400);
    drive_distance(-1000, default_speed);
    slow_servo(arm_servo, 1500, 1000);
    tank_drive_distance(-370, 370, default_speed);
    line_square_up(default_speed);
}
//from centerline
void place_red_block_1(){
    slow_servo(arm_servo, ARM_UP, 500);
    //turn around & back through poms
    drive_distance(5550, default_speed);
    tank_drive_distance(0,2300,default_speed);
    line_square_up(-default_speed);

    //go up to airlock, turn and release block
    drive_distance(-2200, default_speed);
    tank_drive_distance(1100, -1100, default_speed/2);
    drive_distance(1150, default_speed);
    slow_servo(claw_servo, CLAW_OPEN, 500);
    tank_drive_distance(25, -25, default_speed/2);
    ao();
    drive_distance(-1150, default_speed);
    tank_drive_distance(-1100, 1100, default_speed/2);
}
//assumes arm is up
void get_red_block_2(){
    slow_servo(arm_servo, ARM_PIPE_BLOCK, 500);
    line_square_up(default_speed);
    drive_distance(3000, default_speed);
    set_servo_position(claw_servo, CLAW_BLOCK_CLOSED);
    msleep(300);
}

void place_red_block_2(){
	slow_servo(arm_servo, ARM_PIPE_BLOCK+50, 100);
    line_square_up(-default_speed);
    //back to airlock
    drive_distance(-2200, default_speed);
    slow_servo(arm_servo, ARM_UP, 800);
    tank_drive_distance(1320, -1320, default_speed/2);
    drive_distance(750, default_speed);
    slow_servo(claw_servo, 900, 500);
    /*tank_drive_distance(25, -25, default_speed/2);
    ao();
    drive_distance(-800, default_speed);*/
}
/*void turn_airlock(){
    drive_distance(-300, default_speed);
    set_servo_position(claw_servo, CLAW_AIRLOCK_CLOSED);
    slow_servo(arm_servo, ARM_UP, 500);
    //tank_drive_distance(100, -100, default_speed);
    drive_distance(1600, default_speed);
    tank_drive_distance(-600, 600, 200);
}*/
//calibrates and waits for light
void get_ready(){
	enable_servos();
    clear_motor_position_counter(left_motor);
    clear_motor_position_counter(right_motor);
    display_printf(0,0,"Initializing servos");
    set_servo_position(claw_servo, CLAW_BLOCK_CLOSED);
    slow_servo(arm_servo, ARM_UP, 1000);
    display_printf(0,1, "Press A to calibrate tophats, or B to skip");
    while(!(a_button()||b_button())){
    	msleep(1);
    }
    if(a_button()){
        display_clear();
    	while(a_button()) msleep(5);
    	tophat_settings = calibrate_tophats();
    }
    display_clear();
    while (a_button()||b_button()) msleep(50);//debounce buttons
    display_printf(0,0, "Press A to wait for light, or B to skip");
    while(!(a_button()||b_button())){
    	msleep(1);
    }
    if(a_button()){
        display_clear();
        while(a_button()) msleep(5);
    	wait_for_light(light_sensor);
    }
    shut_down_in(118);//leave 2 seconds for coming to rest
}
//This is the "main main". 34 points with low failure chance 
//starting position: rear end lined up with the centerline,
//can just barely fit 1 red block beween pvc and wheel
int main()
{
    get_ready();
    to_centerline();
    place_red_block_1();
	get_red_block_2();
    place_red_block_2();
    return 0;
}

