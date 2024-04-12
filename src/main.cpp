#include "main.h"

/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////


// Chassis constructor
ez::Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is used as the sensor
  {-17, -18, -19}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is used as the sensor
  ,{14, 15, 16}

  // IMU Port
  ,11

  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
  ,4.125

  // Cartridge RPM
  ,200

  // External Gear Ratio (MUST BE DECIMAL) This is WHEEL GEAR / MOTOR GEAR
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 84/36 which is 2.333
  // eg. if your drive is 60:36 where the 36t is powered, your RATIO would be 60/36 which is 0.6
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 36/60 which is 0.6
  ,1.25
);



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();
  
  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0); // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
    Auton("\n\ntest num one", testing)
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset(); // Resets PID targets to 0
  chassis.drive_imu_reset(); // Reset gyro position to 0
  chassis.drive_sensor_reset(); // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency
  
  ez::as::auton_selector.selected_auton_call(); // Calls selected auton from autonomous selector
}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

#define DIGITAL_SENSOR_PORT1 'G'
#define DIGITAL_SENSOR_PORT2 'H'

// controller
pros::Controller master (pros :: E_CONTROLLER_MASTER);

// motors
pros::Motor intake_motor (5, pros::E_MOTOR_GEAR_BLUE);
pros::Motor fly_wheel_motor (10, pros::E_MOTOR_GEAR_BLUE);

// pistons
pros::ADIDigitalOut pneumatic_piston1 (DIGITAL_SENSOR_PORT1);
pros::ADIDigitalOut pneumatic_piston2 (DIGITAL_SENSOR_PORT2);

void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);
  
  bool piston_pos1 = false;
  bool piston_pos2 = true;
  bool piston_pressing = false;
  bool flywheel_run = false;
  
  pneumatic_piston1.set_value(piston_pos1);
  pneumatic_piston2.set_value(piston_pos2);
  // for next day print the values
  while (true) {

    chassis.opcontrol_arcade_standard(ez::SPLIT); // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE); // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT); // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE); // Flipped single arcade

    // intake
    if (master.get_digital(DIGITAL_L1)){
      intake_motor.move(110);

    } else if (master.get_digital(DIGITAL_R1)){
      intake_motor.move(-110);
    
    } else {
      intake_motor.brake();
    }

    // piston 
    if (master.get_digital_new_press(DIGITAL_DOWN)){
      piston_pos1 = !piston_pos1;
      pneumatic_piston1.set_value(piston_pos1);
    } 

    if (master.get_digital_new_press(DIGITAL_RIGHT)){
      piston_pos2 = !piston_pos2;
      pneumatic_piston2.set_value(piston_pos2);
    }

    if (master.get_digital_new_press(DIGITAL_B)){
      if (piston_pos1 && piston_pos2 == false){
        piston_pos1 = false; 
        piston_pos2 = true;

        pneumatic_piston1.set_value(piston_pos1);
        pneumatic_piston2.set_value(piston_pos2);
      } else {
        piston_pos1 = true;
        piston_pos2 = false;
        
        pneumatic_piston1.set_value(piston_pos1);
        pneumatic_piston2.set_value(piston_pos2);
      }
    }

  // fly_wheel
    // if (master.get_digital_new_press(DIGITAL_R2)){
    //   flywheel_run = !flywheel_run;
    // } 

    // if (master.get_digital(DIGITAL_L2) || flywheel_run){  
    //   fly_wheel_motor.move(-110);
    // } else {
    //   fly_wheel_motor.move(0);
    // }

    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
