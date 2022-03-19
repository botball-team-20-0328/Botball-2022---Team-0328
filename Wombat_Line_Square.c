#include <kipr/wombat.h>
//define ports
#define right_motor 0
#define left_motor 1
#define right_tophat 0
#define left_tophat 1
#define claw_min_height 1000
#define claw_max_height 2029
struct ts {
    int right_white;
    int right_black;
    int left_white;
    int left_black;
};
//save these from the calibrate dialog
//DONT LEAVE THEM AS THEY ARE IF YOU COPY PASTE THIS
//order is as struct definition
struct ts tophat_settings = {
    1970,
    3975,
    2750,
    4036
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
    display_printf(0,0,"Place both tophats over black  ");
    display_printf(0,1,"Press A when finished ");
    while(!a_button_clicked()){
        display_printf(0,2,"right value: %d    ", analog(right_tophat));
        display_printf(0,3,"left value: %d    ", analog(left_tophat));
        msleep(5);
    }
    output.right_black = analog(right_tophat);
    output.left_black = analog(left_tophat);

    return output;
}
void tank_drive(int right, int left){
    mav(right_motor, right);
    mav(left_motor, left);
}

void line_square_up(int maxSpeed){
    //slopes for linear function between light and speed

    float right_slope = 1.5f*maxSpeed/(tophat_settings.right_white-tophat_settings.right_black);
    float left_slope = 1.5f*maxSpeed/(tophat_settings.left_white-tophat_settings.left_black);

    int right, left;
    float right_speed;
    float left_speed;
    //float right_between = (tophat_settings.right_black-tophat_settings.right_white)/2.0f;
    //float left_between = (tophat_settings.left_black-tophat_settings.left_white)/2.0f;
    while(analog(right_tophat)<tophat_settings.right_black&&analog(left_tophat)<tophat_settings.left_black){    
        tank_drive(maxSpeed, maxSpeed);
        msleep(5);
    }
    
    do{
        right = analog(right_tophat);
        left = analog(left_tophat);

        display_printf(0,0,"right value: %d    ", right);
        display_printf(0,1,"left value: %d    ", left);
        display_printf(0,2,"right slope: %.4f    ", right_slope);
        display_printf(0,3,"left slope: %.4f    ", left_slope);
        right_speed = right_slope * (right-tophat_settings.right_white) + maxSpeed;
        left_speed = left_slope*(left-tophat_settings.left_white) + maxSpeed;
        tank_drive(right_speed, left_speed);
        display_printf(0,6,"right motor: %.1f    ", right_speed);
        display_printf(0,7,"left motor: %.1f    ", left_speed);
        msleep(10);
    }while(
        right_speed>7||right_speed<-7
        ||left_speed>7||left_speed<-7
    );
    ao();
}
