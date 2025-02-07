#include "robot_config.h"
#include "vex.h"
#include "driving_config.h"


int left_speed = 0;
int right_speed = 0;
bool hook_state = false;

#include "vex.h"
using namespace vex;

//Function for tank drive
// void tank_drive() {
//     int right_speed = Controller.Axis2.position(); // Right joystick for right drive
//     int left_speed = Controller.Axis3.position();  // Left joystick for left drive

//     // Spin motors
//     left_drive.spin(forward, left_speed, rpm);
//     right_drive.spin(forward, right_speed, rpm);
// // }
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
//}
void tank_drive() {
    // Read joystick positions
    int targetRightSpeed = Controller.Axis2.position(); // Right joystick for right drive
    int targetLeftSpeed  = Controller.Axis3.position();  // Left joystick for left drive

    // Static variables to hold the current motor speeds (persist across function calls)
    static int currentRightSpeed = 0;
    static int currentLeftSpeed  = 0;

    // Define the step size for deceleration (ramp down)
    const int rampStep = 15;  // Adjust as needed for a smoother deceleration

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

    int amount = 5;
        if (fabs(currentRightSpeed) <= amount && fabs(currentLeftSpeed) <= amount) {
        // Stop motors and apply brake
        left_drive.stop(brake);
        right_drive.stop(brake);
    }

    // Spin motors with the computed speeds
    left_drive.spin(forward, currentLeftSpeed, rpm);
    right_drive.spin(forward, currentRightSpeed, rpm);
}

// Function for toggling the hook
void hook_start() {
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

// Function for intake motor control
void intake_in() { 
    if (Controller.ButtonY.pressing()) {
        intake_motor.spin(forward, 60, pct);
    } else if (Controller.ButtonB.pressing()) {
        intake_motor.spin(reverse, 60, pct);
    } else {
        intake_motor.stop();
    }
}

// Function for intake and chain control
void intakeNchain() {
    if (Controller.ButtonL1.pressing()) {
        intake_motor.spin(reverse, 100, pct); 
        chain_motor.spin(reverse, 80, pct);
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
        hook_start();      // Hook control
        intakeNchain();    // Intake and chain control
        // // drive_distance((32.2*7.42), 25, 1);
        // // turn_to_angle(90, 30, 0.5);    


        vex::task::sleep(20);  // Prevent CPU overload
    }
}

