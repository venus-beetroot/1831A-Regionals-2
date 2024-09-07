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



void pre_auton(void) 
{
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  

  Intake.setVelocity(100, percent);
  chassis.setTurnVelocity(100, percent);
  chassis.setDriveVelocity(100, percent);
  Clamp.set(false);
  Inertial.calibrate();
  Inertial.setHeading(0, degrees);
  
  
  


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

  chassis.setHeading(0, degrees);
  chassis.turnToHeading(315, degrees, true);
  chassis.driveFor(reverse, 38, inches);

// Clamping and outtaking ring
  Clamp.set(true);
  Intake.spin(forward);
  wait(3, seconds);
  Intake.stop();
  Clamp.set(false);

// Driving away to stop touching Mogo
  chassis.driveFor(forward, 10, inches);
  chassis.turnToHeading(0, degrees, true);

  //Grab First Goal and Score Preload
  // Drivetrain.driveFor(reverse, 35, inches);
  // Clamp.set(false);
  // wait(0.5, sec);
  // // Intake.spin(forward, 200, percent);
  // Intake.spin(forward, 200, percent);
  // wait(0.5, sec);

  // //Release First Goal, grab a second ring onto intake
  // Inertial.setHeading(-90, degrees); 
  // turnToHeading(-90);
  // // Drivetrain.turnFor(left, 90, degrees);
  // Clamp.set(true);
  // wait(0.5, sec);
  // Drivetrain.driveFor(forward, 15, inches);
  // Intake.spin(forward, 50, percent);
  // wait(0.5, sec);

  // //Go and grab second goal
  // Inertial.setHeading(0, degrees); 
  // // Drivetrain.turnFor(left, 90, degrees);
  // turnToHeading(0);
  // Drivetrain.driveFor(reverse, 10, inches);
  // Clamp.set(false);
  // wait(0.5, sec);


  // //score ring
  // Intake.spin(forward, 100, percent);
  // Drivetrain.driveFor(forward, 5, inches);
  
  // wait(0.5, sec);


  // //Turn and go to middle stake and touch it
  // Inertial.setHeading(90, degrees); 
  // // Drivetrain.turnFor(right, 90, degrees);
  // turnToHeading(90);
  // Drivetrain.driveFor(forward, 20, inches);  
  // wait(0.5, sec);

  
  
  
  

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

void usercontrol(void) 
{
  // User control code here, inside the loop
  while (1) 
  {
    chassis.setTurnVelocity(100, percent);
    
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
  while (true) 
  {
    wait(100, msec);
  }
}

void turnToHeading(double targetHeading) 
{
  // Set initial turn velocity (adjust as needed)
  chassis.setTurnVelocity(50, percent);

  // Loop until heading error is within tolerance
  while (std::abs(targetHeading - Inertial.heading()) > 3) 
  {  // Tolerance of 3 degrees
    double headingError = targetHeading - Inertial.heading();
    
    // Normalize heading error
    if (headingError > 180) 
    {
      headingError -= 360;
    } 
    else if (headingError < -180) 
    {
      headingError += 360;
    }

    // Adjust turn velocity based on heading error
    chassis.setTurnVelocity(std::abs(headingError) * 0.5, percent); // Linearly adjust velocity

    // Turn the drivetrain
    if (headingError > 0) 
    {
      chassis.turn(right);
    } 
    else 
    {
      chassis.turn(left);
    }

    // Short wait (adjust as needed)
    wait(0.05, sec);
  }

  // Stop the drivetrain
  chassis.stop();
}


