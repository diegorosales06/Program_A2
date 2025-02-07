#include "autonomous.h"
#include "robot_config.h"


bool enableAutonDrive = true;


void set_auton(){
    inertial_heading.calibrate();
    while(inertial_heading.isCalibrating()){
        vex::task::sleep(2000);
    }
    

}

void turn_to_angle(float target_angle, float speed, float AcceptableError){
    double kP = 0.25; //for error  
    double kI = 0.09;   // for integral
    double kD = 0.002;   
    float prevError = 0; // position 20 miliseconds ago for derivative

    // FOR PROPORTIONAL VALUE
    float P_Value = 0; //P
    float angular_error = 0; // error

    // FOR D
    float current_heading = 0; //angle of robot
    float D_Value = 0;
    float LoopTime = 0.02;

    float I_Value = 0;

    int RightMotor_turnspeed = 0;
    int LeftMotor_turnspeed = 0;
    inertial_heading.resetHeading();// resets the heading of the Inertial Sensor to 0.


    while (true){   
        // LoopTime = Timer.value();
        current_heading = inertial_heading.heading();
        angular_error = target_angle - current_heading;

        //find most effiecent way to turn (cw or ccw)//////


       if (angular_error > 180) {
                angular_error -= 360;  // Turn counter-clockwise
            } else if (angular_error < -180) {
                angular_error += 360;  // Turn clockwise
            }

        // if (target_angle <= (current_heading + 180)){ // counter clockwise turn
        //     angular_error = angular_error - 360;

        // }   else if (target_angle >= (current_heading +180)){ // clockwise turn
        //         angular_error = angular_error + 360;

        // }
        // ////////////////////

 

        // P = Proportional
        P_Value = kP*angular_error;
     
        //D = Derivative;

        I_Value += (kI * angular_error * LoopTime);

        D_Value = kD * ((angular_error - prevError) / LoopTime);

        prevError = angular_error;

        RightMotor_turnspeed = speed - (P_Value + D_Value + I_Value);

        LeftMotor_turnspeed = speed + (P_Value + D_Value + I_Value);


        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Heading: %.2f", current_heading);
        Brain.Screen.setCursor(2, 1); // Move to the second row

        Brain.Screen.print("Right: %.2f", RightMotor_turnspeed);
        Brain.Screen.setCursor(3, 1); // Move to the second row

        Brain.Screen.print("Left: %.2f", LeftMotor_turnspeed);

        Brain.Screen.setCursor(4, 1); // Move to the second row
        Brain.Screen.print("P_Value: %.2f", P_Value);
        Brain.Screen.setCursor(5, 1); // Move to the second row
        Brain.Screen.print("error: %.2f", angular_error);

    
        left_drive.spin(fwd, LeftMotor_turnspeed, pct);
        right_drive.spin(fwd, RightMotor_turnspeed, pct);

        if (fabs(angular_error) < AcceptableError){
            left_drive.stop();
            right_drive.stop();

            break;
        }
        // Timer.clear();
        vex::task::sleep(20);  // Delay to debounce



    }
} 


// /// clockwise = positive, cc = negative

void drive_distance(float target_distance, float speed, float AcceptableError) {
    // PID Constants
    double kP = 0.5;
    double kI = 0.03;   // Integral gain
    double kD = 0.0;     // Derivative gain

    float P_Value = 0;
    float I_Value = 0;
    float D_Value = 0;
    float prevError = 0;

    float I_Limit = 50;  // Prevent integral windup

    // Reset motor encoders
    left_drive.setPosition(0, degrees);
    right_drive.setPosition(0, degrees);

    float LoopTime = 0.02;  // 20ms loop time

    float lateral_error = 0;
    float RightMotor_turnspeed = 0;
    float LeftMotor_turnspeed = 0;

    float avgPosition = 0;
    float RightMotorPosition = 0;
    float LeftMotorPosition = 0;

    while (true) {
        // Get motor positions
        LeftMotorPosition = left_drive.position(degrees);
        RightMotorPosition = right_drive.position(degrees);

        // Calculate average position
        avgPosition = (LeftMotorPosition + RightMotorPosition);
        lateral_error = target_distance - avgPosition;

        //printf("lateral_error: %f\n", lateral_error);
        //printf("avgPosition: %f\n", avgPosition);
        printf("RightPosition: %f\n", RightMotorPosition);
        

        // PID Calculations
        P_Value = lateral_error * kP;
        I_Value += (kI * lateral_error * LoopTime);

        // Prevent integral windup
        if (fabs(lateral_error) < AcceptableError) {
            I_Value = 0;  // Reset integral term when near the target
        } else if (I_Value > I_Limit) {
            I_Value = I_Limit;
        } else if (I_Value < -I_Limit) {
            I_Value = -I_Limit;
        }

        D_Value = kD * ((lateral_error - prevError) / LoopTime);

        // Compute motor speeds (PID directly controls speed)
        LeftMotor_turnspeed = P_Value + I_Value + D_Value;
        RightMotor_turnspeed = P_Value + I_Value + D_Value;

        // Clamp motor speeds to max speed limit
        if (LeftMotor_turnspeed > speed) LeftMotor_turnspeed = speed;
        if (LeftMotor_turnspeed < -speed) LeftMotor_turnspeed = -speed;
        if (RightMotor_turnspeed > speed) RightMotor_turnspeed = speed;
        if (RightMotor_turnspeed < -speed) RightMotor_turnspeed = -speed;


        if (fabs(lateral_error) < AcceptableError || lateral_error * prevError < 0) {
                I_Value = 0;  // Reset integral term
            }

        // Apply motor speeds (automatically handles forward & reverse)
        //left_drive.spin(fwd, LeftMotor_turnspeed, pct);
        right_drive.spin(fwd, RightMotor_turnspeed, pct);

        // Exit if error is within acceptable range
        if (fabs(lateral_error) <= AcceptableError) {
            left_drive.stop(brake);
            right_drive.stop(brake);
            break;
        }

        prevError = lateral_error;
        vex::task::sleep(20);  // 20ms delay to prevent CPU overload
    }
}

void auton_clamp_engage(){
    hook.set(true);
}

void auton_clamp_disengage(){
    hook.set(false);
}


 // Global variable to store parameter

int auton_run_intake_task() {
    int delay = 7000;
    float intake_speed = 80;
    intake_motor.spin(reverse, 100, pct); 
    chain_motor.spin(reverse, 80, pct);
    vex::task::sleep(delay);  // Run for 4 seconds
    intake_motor.stop();
    chain_motor.stop();
    return 0;
}

void auton_run_intake(float run_time) {
    intake_motor.spin(reverse, 100, pct); 
    chain_motor.spin(reverse, 100, pct);

    vex::task::sleep(run_time * 1000); // Convert seconds to milliseconds

    intake_motor.stop();
    chain_motor.stop();


}

void NONpid(float distance, float speed){
    float needed_revs;

    needed_revs = distance/3.33;
    //MotorGroup.spinFor(direction, value, units, velocity, units_v, wait)
    //motor.spinFor(directionType, rotation, rotationUnits, velocity, velocityUnits, wait)

    right_drive.spinFor(fwd, needed_revs, rev, speed, velocityUnits::pct, true); 
    left_drive.spinFor(fwd, needed_revs, rev, speed, velocityUnits::pct, false);
}
void autonomous_drive() {
    // Reset the motor position to 0 degrees
    //set_auton();
    //drive_distance(360, 10, 2);

    drive_distance(-300, 50, 2);
    auton_clamp_engage();
    vex::task::sleep(1000);
    drive_distance(-315, 50, 2);
    auton_run_intake(4);
    turn_to_angle(90, 25, 2);
    vex::task intake_task(auton_run_intake_task);
    drive_distance(400, 25, 2);
}
