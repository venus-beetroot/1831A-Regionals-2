/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Haoran Fang                                               */
/*    Created:      Sep 06, 2024                                              */
/*    Description:  Regionals 2 Competition                                   */
/*    Template:     VEX Competition Template                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    20, 19, 17, 16  
// Intake               motor_group   9, 10           
// Clamp                digital_out   H               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robot-config.h"
#include <cmath>
#include <cstdlib>

using namespace vex;

// A global instance of competition
competition Competition;

void turnToHeading(double targetHeading);


// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/



void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // motor_group RightDriveSmart = motor_group(leftMotorA, leftMotorB, leftMotorC);
  // motor_group LeftDriveSmart = motor_group(rightMotorA, rightMotorB, rightMotorC);
  // drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 40, mm, 1);
  // motor IntakeMotorA = motor(PORT9, ratio18_1, false);
  // motor IntakeMotorB = motor(PORT10, ratio18_1, true);
  // motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);
  // digital_out Clamp = digital_out(Brain.ThreeWirePort.H);

  RightDriveSmart.setVelocity(100, percent);
  LeftDriveSmart.setVelocity(100, percent);
  Intake.setVelocity(100, percent);
  Drivetrain.setTurnVelocity(100, percent);
  Drivetrain.setDriveVelocity(100, percent);
  Clamp.set(true);

  Inertial.setHeading(0, degrees);
  Inertial.calibrate();
  
  
  


  //set inertial for dt turning if inertial sensors are added to the robot
  //inertial Inertial = inertial(PORT*);
  //start calibration (must be done when sensor isnt moving)
    //Inertial.calibrate();
  //inertial is calibrating indicator
    //Inertial.calibrate(seconds);
    // while(Inertial.isCalibrating()){
    // Brain.Screen.clear();
    // Brain.Screen.print("Inertial Calibrating");
    // wait(50, MSEC);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/



void autonomous(void) 
{

  //Grab First Goal and Score Preload
  Drivetrain.driveFor(reverse, 35, inches);
  Clamp.set(false);
  // Intake.spin(forward, 200, percent);
  Intake.spin(forward, 200, percent);
  wait(0.5, sec);

  //Release First Goal, grab a second ring onto intake
  Inertial.setHeading(-90, degrees); 
  turnToHeading(-90);
  // Drivetrain.turnFor(left, 90, degrees);
  Clamp.set(true);
  //set inertial for dt turning if inertial sensors are added to the robot
  Drivetrain.driveFor(forward, 15, inches);
  Intake.spin(forward, 50, percent);
  
  wait(0.5, sec);

  //Go and grab second goal
  Inertial.setHeading(0, degrees); 
  // Drivetrain.turnFor(left, 90, degrees);
  turnToHeading(0);
  Drivetrain.driveFor(reverse, 10, inches);
  Clamp.set(false);
  wait(0.5, sec);


  //score ring
  Intake.spin(forward, 100, percent);
  Drivetrain.driveFor(forward, 5, inches);
  
  wait(0.5, sec);


  //Turn and go to middle stake and touch it
  Inertial.setHeading(90, degrees); 
  // Drivetrain.turnFor(right, 90, degrees);
  turnToHeading(90);
  Drivetrain.driveFor(forward, 20, inches);  
  wait(0.5, sec);

  
  
  
  

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  pre_auton();
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}


void turnToHeading(double targetHeading) 
{
  // Set the drivetrain to turn mode
  Drivetrain.setTurnVelocity(50, percent);
 
  // Calculate the difference between the current heading and the target heading
  double headingError = targetHeading - Inertial.heading();
 
  // Normalize the heading error to the range -180 to 180
  if (headingError > 180) {
    headingError -= 360;
  } else if (headingError < -180) {
    headingError += 360;
  }
 
  // Turn the drivetrain until the heading error is within a certain tolerance
  while (std::abs(headingError) > 5) {
    // Update the heading error
    headingError = targetHeading - Inertial.heading();
   
    // Normalize the heading error to the range -180 to 180
    if (headingError > 180) {
      headingError -= 360;
    } else if (headingError < -180) {
      headingError += 360;
    }
   
    // Turn the drivetrain
    if (headingError > 0) {
      Drivetrain.turn(right);
    } else {
      Drivetrain.turn(left);
    }
   
    // Wait for a short period of time before updating the heading error again
    wait(20, msec);
  }
 
  // Stop the drivetrain
  Drivetrain.stop();
}


