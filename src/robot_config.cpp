#include "vex.h"
#include "robot_config.h"

using namespace vex;

brain Brain;

timer Timer;

competition Competition;

controller Controller;

gearSetting ratio = ratio36_1;

motor left_back(PORT1, ratio, false);


motor left_middle(PORT3, ratio, false);

motor left_front(PORT2, ratio, true); // top motor of left drive


motor right_back(PORT4, ratio, true);
motor right_middle(PORT5, ratio, true);
motor right_front(PORT6, ratio, false); // top motor of right drive 

motor intake_motor(PORT8, ratio18_1, false);
//inertial inertial_heading(PORT7);
inertial inertial_heading(PORT7);


motor chain_motor(PORT9, ratio36_1, false);  // dump motor(top of the robot

motor_group left_drive(left_back,left_middle,left_front);

motor_group right_drive(right_back,right_middle,right_front);

digital_out hook(Brain.ThreeWirePort.A); //piston 
//vision::signature darkBlue = vision::signature(1, -3911, -3435, -3673,10879, 11421, 11150,2.5, 0);


//vision Vision1(PORT1);

optical Optical(PORT10);
