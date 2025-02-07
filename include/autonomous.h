#include "vex.h"



extern void turn_to_angle(float target_angle, float speed, float AcceptableError);
extern void drive_distance(float target_distance, float speed, float AcceptableError);
extern void auton_clamp_engage();
extern void auton_clamp_disengage();
extern void auton_run_intake(float run_time);
extern void NONpid(float distance, float speed);
extern void autonomous_drive();