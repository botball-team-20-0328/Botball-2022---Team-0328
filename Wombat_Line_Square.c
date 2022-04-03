#include <kipr/wombat.h>
//define ports
#define right_tophat 0
#define left_tophat 2
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
    mav(left_motor, left*0.96);//correct for slow right motor
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

    do{
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
