//Create library for driving with distances and angles rather than speeds and times
//Author: James Bankhead
//2022 Botball Season

//Every function is prefixed with roomba_
//Why is there no intellisense ...
#ifndef ROOMBA_H
#define ROOMBA_H
#include <kipr/wombat.h>
#ifndef PI
#define PI 3.141592
#endif
//all units in millimeters, degrees, and seconds
const int roomba_dist_between_wheels = 235;

int roomba_left_line_threshold = 0;
int roomba_right_line_threshold = 0;
//clockwise is negative degrees
void roomba_turn_in_place(int degrees, int speed){
    set_create_total_angle(0);
    if(degrees>0){
        create_drive_direct(-speed, speed);
        int i = 0;
        while(get_create_total_angle()<degrees){
            if(i%10==0){
                printf("angle: %d\n", get_create_total_angle());
            }
            msleep(5);
            i++;
        }
    }
    else{
        create_drive_direct(speed, -speed);
        while(get_create_total_angle()>degrees){
            msleep(5);
        }
    }
    create_stop();
}
//drives straight (using angle detection);
//distance in mm, speed in mm/sec
void roomba_drive_straight(int distance, int speed){
    set_create_total_angle(0);
    set_create_distance(0);
    create_drive_direct(speed,speed);
    if(speed>0){
        while(get_create_distance()<distance){
            int angle_adjust = get_create_total_angle();
            create_drive_direct(speed+angle_adjust,speed-angle_adjust);
            msleep(5);
        }
    }else{
        while(get_create_distance()>distance){
            int angle_adjust = get_create_total_angle();
            create_drive_direct(speed-angle_adjust,speed+angle_adjust);
            msleep(5);
        }
    }
    create_stop();
}
//can only do positive speed
void roomba_turn_with_radius(int angle, int radius, int speed){
    set_create_total_angle(0);
    set_create_distance(0);
    int circumference = radius * 2 * PI;
    if(angle>0){
        while(get_create_total_angle()<angle){
            int desired_angle = 360*get_create_distance()/circumference;
            int angle_adjustment = desired_angle-get_create_total_angle();
            int speed_change = angle_adjustment*angle_adjustment;
            create_drive_direct(speed-speed_change, speed+speed_change);
            msleep(5);
            printf("angle: %d   ", get_create_total_angle());
        }
    }
    else{
        while(get_create_total_angle()<angle){
            int desired_angle = 360*get_create_distance()/circumference;
            int angle_adjustment = desired_angle-get_create_total_angle();
            int speed_change = angle_adjustment*angle_adjustment;
            create_drive_direct(speed-speed_change, speed+speed_change);
            msleep(5);
        }
    }
}
void roomba_calibrate_line_follower(){
    while(!a_button()){
    	display_printf(0,0,"Place both front cliff sensors over white");
        display_printf(0,1, "Press A when finished");
        display_printf(0,2, "right front cliff value: %d",  get_create_rfcliff_amt());
        display_printf(0,3, "left front cliff value: %d", get_create_lfcliff_amt());
        msleep(1);
    }
    int right_white = get_create_rfcliff_amt();
    int left_white = get_create_lfcliff_amt();
    display_clear();
    display_printf(0,0, "White values are: left %d, right %d \t\t\t\t\t\t\t\t", left_white, right_white);
    msleep(500);
    display_clear();
    while(!a_button()){
    	display_printf(0,0,"Place right front cliff sensor over black");
        display_printf(0,1,"Press A when finished");
        display_printf(0,2, "right front cliff value: %d\t\t\t\t",  get_create_rfcliff_amt());
        msleep(1);
    }
    int right_black = get_create_rfcliff_amt();
    display_clear();
    display_printf(0,0,"Right black value: %d\t\t\t\t\t\t\t", right_black);
    msleep(1000);
    display_clear();
    while(!a_button()){
    	display_printf(0,0,"Place left front cliff sensor over black");
        display_printf(0,1,"Press A when finished");
        display_printf(0,2, "left front cliff value: %d",  get_create_lfcliff_amt());
        msleep(1);
    }
    display_clear();
    int left_black = get_create_lfcliff_amt();
    display_printf(0,0,"Left black value: %d\t\t\t\t\t\t\t", right_black);
    msleep(1000);
    display_clear();
    roomba_right_line_threshold = (right_white+right_black)/2;
    roomba_left_line_threshold = (left_white+left_black)/2;
    display_printf(0,0,"calibration complete\t\t\t\t\t\t");
    display_printf(0,1, "right threshold: %d", roomba_right_line_threshold);
    display_printf(0,2, "left threshold: %d", roomba_left_line_threshold);
    msleep(2000);
    display_clear();
}
//always calibrate first
//time still in milliseconds
void roomba_follow_line_timed(int time, int speed){
	set_create_distance(0);
    double time_start = seconds();
    printf("start: %f", time_start);
    double time_end = time_start+(double)time/1000;
    printf("end: %f", time_end);
    while(seconds()<time_end){
    	int left_cliff = get_create_lcliff_amt();
        int right_cliff = get_create_rcliff_amt();
        if(left_cliff<roomba_left_line_threshold){
            create_drive_direct(speed*0.8, speed);
            display_printf(0,0,"left too much\t\t");
        }
        else if(right_cliff<roomba_right_line_threshold){
            create_drive_direct(speed, speed*0.8);
            display_printf(0,0,"right too much\t\t");
        }
        else{
        	create_drive_direct(speed, speed);
            display_printf(0,0,"just right\t\t\t\t");
        }
        msleep(5);
    }
}
#endif
