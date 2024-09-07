#include "vex.h"
#include <cmath>
#include <cstdlib>


using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;



// VEXcode device constructors
motor leftMotorA = motor(PORT20, ratio6_1, false);
motor leftMotorB = motor(PORT19, ratio6_1, false);
motor leftMotorC = motor(PORT18, ratio6_1, false);
motor_group RightDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
motor rightMotorA = motor(PORT17, ratio6_1, true);
motor rightMotorB = motor(PORT16, ratio6_1, true);
motor rightMotorC = motor(PORT15, ratio6_1, true);
motor_group LeftDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);

inertial Inertial = inertial(PORT5);

smartdrive chassis = smartdrive(LeftDriveSmart, RightDriveSmart, Inertial, 319.19, 295, 40, mm, 1);

motor IntakeMotorA = motor(PORT9, ratio18_1, false);
motor IntakeMotorB = motor(PORT10, ratio18_1, true);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);
digital_out Clamp = digital_out(Brain.ThreeWirePort.H);
controller Controller1 = controller(primary);


void configuremotors(){
  Intake.setVelocity(100,percent);
  leftMotorA.setVelocity(100,percent);
  leftMotorB.setVelocity(100,percent);
  leftMotorC.setVelocity(100,percent);
  rightMotorA.setVelocity(100,percent);
  rightMotorB.setVelocity(100,percent);
  rightMotorC.setVelocity(100,percent);

}






// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
// bool Controller1RightShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

bool clampActive = false;

void clampChange() {
  if (clampActive == false){
    clampActive = true;
  } else
  {
    clampActive = false;
  }
}

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      if(Controller1.ButtonL1.pressing()){
        clampActive = true;
      } else if (Controller1.ButtonL2.pressing()){
        clampActive = false;
      }

      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = Axis3
      // right = Axis2
      int drivetrainLeftSideSpeed = Controller1.Axis3.position();
      int drivetrainRightSideSpeed = Controller1.Axis2.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller1) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller1 = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller1) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller1 = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller1 = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonR1/ButtonR2 status to control Intake
    

      if(Controller1.ButtonR1.pressing()){
        Intake.spin(forward, 100, percent);
      } else if(Controller1.ButtonR2.pressing())
      {
        Intake.spin(reverse, 100, percent);
      }
      else{
        Intake.stop();
      
      }
      if(clampActive==true){
        Clamp.set(true);
      } else{
        Clamp.set(false);
      }

      
       //else if (!Controller1RightShoulderControlMotorsStopped) {
      //   Intake.stop();
      //   // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
      //   Controller1RightShoulderControlMotorsStopped = true;
      // }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;


  // Define the inertial sensor and drivetrain objects

// Function to turn the drivetrain to a specific heading


// // Define the robot's dimensions
// const double wheelDiameter = 4.0;
// const double wheelBase = 10.0;

// // Function to drive the robot forward a certain distance
// void driveForward(double distance) {
//   // Calculate the number of rotations required
//   double rotations = distance / (wheelDiameter * M_PI);

//   // Set the target rotation for the motor groups
//   leftMotors.resetRotation();
//   rightMotors.resetRotation();
//   leftMotors.setTargetRotation(rotations, rotationUnits::rev);
//   rightMotors.setTargetRotation(rotations, rotationUnits::rev);

//   // Spin the motors until they reach their target rotation
//   while (leftMotors.isSpinning() || rightMotors.isSpinning()) {
//     wait(5, msec);
//   }
// }

// // Function to drive the robot backward a certain distance
// void driveBackward(double distance) {
//   // Call the driveForward function with a negative distance
//   driveForward(-distance);
// }

// // Function to turn the robot a certain angle
// void turn(double angle) {
//   // Calculate the number of rotations required
//   double rotations = (angle * wheelBase) / (wheelDiameter * M_PI);

//   // Set the target rotation for the motor groups
//   leftMotors.resetRotation();
//   rightMotors.resetRotation();
//   leftMotors.setTargetRotation(-rotations, rotationUnits::rev);
//   rightMotors.setTargetRotation(rotations, rotationUnits::rev);

//   // Spin the motors until they reach their target rotation
//   while (leftMotors.isSpinning() || rightMotors.isSpinning()) {
//     wait(5, msec);
//   }
// }

// // Function to calibrate the inertial sensor
// void calibrateInertial() {
//   // Start the calibration process
//   inertialSensor.calibrate();

//   // Wait for the calibration to finish
//   while (inertialSensor.isCalibrating()) {
//     wait(25, msec);
//   }
// }

// // Main function
// int main() {
//   // Calibrate the inertial sensor
//   calibrateInertial();

//   // Drive forward 10 inches
//   driveForward(10);

//   // Turn 90 degrees to the right
//   turn(90);

//   // Drive forward 10 inches
//   driveForward(10);

//   // Wait for the program to finish
//   while (true) {
//     wait(25, msec);
//   }
// }



}

void controlDrivetrain(){
  while(true){
    int a3value = Controller1.Axis3.position(percentUnits::pct);
    
    int a2Value = Controller1.Axis2.position(percentUnits::pct);

    // Control LD (left drive) motors
    while (a3value > 3) {  // Joystick is pushed up
      LeftDriveSmart.spin(forward, a3value, percentUnits::pct);
    } while (a3value < -3) {  // Joystick is pulled down
      LeftDriveSmart.spin(reverse, -a3value, percentUnits::pct);
    } while(a3value == 1 || a3value == -1) {  // Joystick is near the center, stop the motor
      LeftDriveSmart.stop();
    }

    // Control RD (right drive) motors
    while (a2Value > 3) {  // Joystick is pushed up
      RightDriveSmart.spin(forward, a2Value, percentUnits::pct);
    } while (a2Value < -3) {  // Joystick is pulled down
      RightDriveSmart.spin(reverse, -a2Value, percentUnits::pct);
    } while(a2Value == 1 || a2Value == -1) {  // Joystick is near the center, stop the motor
      RightDriveSmart.stop();
    }

    // Add a short delay to prevent overwhelming the CPU
    vex::task::sleep(5);

    // bool mogoState = false; // Initialize mogo state to false (off)

    while (true) {
      if (Controller1.ButtonL2.pressing()) 
      {
      Clamp.set(true);
      
      wait(5, msec);
      } 
      else if (Controller1.ButtonL1.pressing()) 
      {
      Clamp.set(false);
      
      wait(5, msec);
      }
      wait(5, msec);
    }
  }
}



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);
}