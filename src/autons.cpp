#include "main.h"

/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 127;  
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

// 60-90;
// 110-127
// pros::delay()
///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(3, 0, 20);
  chassis.pid_drive_constants_set(30, 0, 10);
  chassis.pid_turn_constants_set(3, 0, 20);
  chassis.pid_swing_constants_set(5, 0, 30);

  chassis.pid_turn_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms, 750_ms);
  chassis.pid_swing_exit_condition_set(300_ms, 3_deg, 500_ms, 7_deg, 750_ms, 750_ms);
  chassis.pid_drive_exit_condition_set(300_ms, 1_in, 500_ms, 3_in, 750_ms, 750_ms);

  chassis.slew_drive_constants_set(7_in, 80);
}

void testing() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED);
  chassis.pid_wait();
}
// Make your own autonomous functions here!
// . . .