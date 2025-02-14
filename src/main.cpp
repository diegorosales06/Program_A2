/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       cloud                                                     */
/*    Created:      1/10/2025, 1:51:37 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "driving_config.h"
#include "robot_config.h"
#include "autonomous.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
// void autonomous_drive(){
//     turn_to_angle(180, 30, 0.5);
    
//     //32.2 is one rotation of the motor
//     //3.33 inches
//     //turn_to_angle(90, 30, 0.5);
//     //turn_to_angle(90, 30, 0.5);
    

// }

// **Main Loop (Keeps Everything Running)**
// int main() {
//     if (Competition.isAutonomous() ){
//         autonomous(autonomous_drive());

//     }
//     else if (Competition.isDriverControl() ){
//         manualcontrol();

//     }
    
// }

int main() {
    //Competition.autonomous(autonomous_drive);
    //Competition.drivercontrol(manualcontrol);
    //autonomous_drive();
    manualcontrol();

}
