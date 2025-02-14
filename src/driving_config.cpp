#include "robot_config.h"
#include "vex.h"
#include "driving_config.h"


int left_speed = 0;
int right_speed = 0;
bool hook_state = false;
int chain_speed = 80;
int chain_speed_normal = 75;

#include "vex.h"
using namespace vex;

//Function for tank drive
// void tank_drive() {
//     int right_speed = Controller.Axis2.position(); // Right joystick for right drive
//     int left_speed = Controller.Axis3.position();  // Left joystick for left drive

//     // Spin motors
//     left_drive.spin(forward, left_speed, pct);
//     right_drive.spin(forward, right_speed, pct);
// }
// void tank_drive() {
//     int right_speed = Controller.Axis2.position(); // Right joystick for right drive
//     int left_speed = Controller.Axis3.position();  // Left joystick for left drive

//     // Check if joystick input is zero
//     if (right_speed == 0 && left_speed == 0) {
//         // Stop motors and apply brake
//         left_drive.stop(brake);
//         right_drive.stop(brake);
//     } else {
//         // Spin motors with the joystick input
//         left_drive.spin(forward, left_speed, rpm);
//         right_drive.spin(forward, right_speed, rpm);
//     }
// }

int currentRightSpeed = 0;
int currentLeftSpeed = 0;


void tank_drive() {
    // Read joystick positions
    int targetRightSpeed = Controller.Axis2.position(); // Right joystick for right drive
    int targetLeftSpeed  = Controller.Axis3.position();  // Left joystick for left drive

    // Static variables to hold the current motor speeds (persist across function calls)

    // Define the step size for deceleration (ramp down)
    const int rampStep = 12;  // Adjust as needed for a smoother deceleration

    // For the right motor:
    if (targetRightSpeed > currentRightSpeed) {
        // Immediate acceleration
        currentRightSpeed = targetRightSpeed;

    } else if (targetRightSpeed < currentRightSpeed) {
        // Gradual deceleration
        currentRightSpeed -= rampStep;
        if (currentRightSpeed < targetRightSpeed) {
            currentRightSpeed = targetRightSpeed;
        }
    }

    // For the left motor:
    if (targetLeftSpeed > currentLeftSpeed) {
        // Immediate acceleration
        currentLeftSpeed = targetLeftSpeed;
    } else if (targetLeftSpeed < currentLeftSpeed) {
        // Gradual deceleration
        currentLeftSpeed -= rampStep;
        if (currentLeftSpeed < targetLeftSpeed) {
            currentLeftSpeed = targetLeftSpeed;
        }
    }

    int amount = 5;// minimum speed to have the motor stop completely
        if (fabs(currentRightSpeed) <= amount && fabs(currentLeftSpeed) <= amount) {
        // Stop motors and apply brake
        left_drive.stop(brake);
        right_drive.stop(brake);
    }

    // Spin motors with the computed speeds
    left_drive.spin(forward, currentLeftSpeed, pct);
    right_drive.spin(forward, currentRightSpeed, pct);
    printf("RightPosition: %f\n", currentRightSpeed);

}
// void tank_drive() {
//     // Read raw joystick positions (-100 to 100)
//     int rawRightJoystick = Controller.Axis2.position();
//     int rawLeftJoystick  = Controller.Axis3.position();

//     // Apply exponential scaling to joystick input
//     const double exponent = 3.0; // Adjust for sensitivity (higher = more gradual acceleration)
//     const int maxJoystick = 100; // Joystick range
//     const int maxMotorSpeed = 100; // Motor speed range

//     // Normalize joystick input (-1 to 1), apply scaling, and restore range (-100 to 100)
//     int targetRightSpeed = pow(fabs(rawRightJoystick) / maxJoystick, exponent) * maxMotorSpeed;
//     int targetLeftSpeed = pow(fabs(rawLeftJoystick) / maxJoystick, exponent) * maxMotorSpeed;

//     // Restore correct sign
//     targetRightSpeed *= (rawRightJoystick < 0) ? -1 : 1;
//     targetLeftSpeed *= (rawLeftJoystick < 0) ? -1 : 1;

//     // Persistent motor speeds across function calls
//     static int currentRightSpeed = 0;
//     static int currentLeftSpeed = 0;

//     const int rampStep = 8;  // Adjusted for smoother transitions

//     // Smooth acceleration & deceleration logic
//     if (targetRightSpeed > currentRightSpeed) {
//         currentRightSpeed += rampStep;
//         if (currentRightSpeed > targetRightSpeed) {
//             currentRightSpeed = targetRightSpeed;
//         }
//     } else if (targetRightSpeed < currentRightSpeed) {
//         currentRightSpeed -= rampStep;
//         if (currentRightSpeed < targetRightSpeed) {
//             currentRightSpeed = targetRightSpeed;
//         }
//     }

//     if (targetLeftSpeed > currentLeftSpeed) {
//         currentLeftSpeed += rampStep;
//         if (currentLeftSpeed > targetLeftSpeed) {
//             currentLeftSpeed = targetLeftSpeed;
//         }
//     } else if (targetLeftSpeed < currentLeftSpeed) {
//         currentLeftSpeed -= rampStep;
//         if (currentLeftSpeed < targetLeftSpeed) {
//             currentLeftSpeed = targetLeftSpeed;
//         }
//     }

//     int amount = 5; // Threshold for stopping motors
//     if (fabs(currentRightSpeed) <= amount && fabs(currentLeftSpeed) <= amount) {
//         left_drive.stop(brake);
//         right_drive.stop(brake);
//     } else {
//         left_drive.spin(forward, currentLeftSpeed, pct);
//         right_drive.spin(forward, currentRightSpeed, pct);
//     }

//     // Debugging output
//     printf("Joystick Right: %d, Target Right: %d, Current Right: %d\n", 
//            rawRightJoystick, targetRightSpeed, currentRightSpeed);
//     fflush(stdout);
// }

// Function for toggling the hook
void clamp_run() {
    static bool hook_state = false;  // Ensure hook state persists

    if (Controller.ButtonR1.pressing()) {
        hook_state = !hook_state;  // Toggle state
        hook.set(hook_state);  // Activate or deactivate hook

        // Wait for button release to avoid rapid toggling
        while (Controller.ButtonR1.pressing()) {
            vex::task::sleep(20);
        }
    }
}
int intake_sense() {
    int desiredColor = 2;
    double hue = 0.0;  
    int color = 0;  

    // Optical.setLight(ledState::on);
    // Optical.setLightPower(50, percent);
    // this_thread::sleep_for(50); // Give time for light to stabilize

    // Average multiple readings

    // double hue1 = Optical.hue();
    // this_thread::sleep_for(10);
    // double hue2 = Optical.hue();
    // this_thread::sleep_for(10);
    // double hue3 = Optical.hue();
    hue = Optical.hue();

    printf("hue: %f\n", hue);

    if (hue >= 350 && hue <= 15) {
        color = 1; // Red
    } else if (hue >= 185 && hue <= 235) {
        color = 2; // Blue
    } else{
        color = 3;//normal
    }
    //(condition) ? true : false;
    //(color == desiredColor || color == 3)? chain_speed=chain_speed_normal : chain_speed = 100;
    if (color == desiredColor) {
        chain_speed = chain_speed_normal;
        printf("normal");
    } else if(color == 3){
        chain_speed = chain_speed_normal;
        printf("normal");

    }
    else {
        vex::task::sleep(500);
        chain_speed = 0;
        vex::task::sleep(100);
        printf("fast");
    }
    return 0;
}



void armcontrol(){
    static bool arm_state = false;

    if (Controller.ButtonR2.pressing()) {
        arm_state = !arm_state;  // Toggle state
        arm.set(arm_state);  // Activate or deactivate hook

        // Wait for button release to avoid rapid toggling
        while (Controller.ButtonR2.pressing()) {
            vex::task::sleep(20);
        }
    }
}
// Function for intake and chain control
void intakeNchain() {
    if (Controller.ButtonL1.pressing()) {

        intake_motor.spin(reverse, 100, pct); 
        chain_motor.spin(reverse, chain_speed, pct);
    } else if(Controller.ButtonL2.pressing()){
            chain_motor.spin(fwd, 60, pct);
            intake_motor.spin(fwd, 60, pct);

    }else {
        intake_motor.stop();
        chain_motor.stop();
    }
}

void manualcontrol(){
    while (true) {  // Continuous loop
        tank_drive();      // Drive control
        clamp_run();      // Hook control
        intakeNchain();    // Intake and chain control
        armcontrol();

        //vex::task intake_sense(intake_sense); 


        vex::task::sleep(20);  // Prevent CPU overload
    }
}

