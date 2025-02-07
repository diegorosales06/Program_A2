#include "vex.h"
#include "robot_config.h"

using namespace vex;

brain Brain;

timer Timer;

competition Competition;

controller Controller;

gearSetting ratio = ratio36_1;

motor left_back(PORT1, ratio, true);


motor left_middle(PORT2, ratio, true);

motor left_front(PORT3, ratio, false); // top motor of left drive


motor right_back(PORT4, ratio, false);
motor right_middle(PORT5, ratio, false);
motor right_front(PORT6, ratio, true); // top motor of right drive 

motor intake_motor(PORT8, ratio18_1, true);
//inertial inertial_heading(PORT7);
inertial inertial_heading(PORT7);


motor chain_motor(PORT9, ratio36_1, false);  // dump motor(top of the robot

motor_group left_drive(left_back,left_middle,left_front);

motor_group right_drive(right_back,right_middle,right_front);

digital_out hook(Brain.ThreeWirePort.A); //piston 

