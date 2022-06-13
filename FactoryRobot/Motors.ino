/**
 * Our method fired to start the motors. We set different variables for the motors and put them in the correct positions.
 */
void startup() {
  // turn on motor 1 (arm motor)
  arm_motor.setSpeed(armSpeed); // set the speed to armSpeed
  arm_motor.run(RELEASE); // no movement yet

  // turn on motor 2 (slide motor)
  slide_motor.setSpeed(slideSpeed); // set the speed to slideSpeed
  slide_motor.run(RELEASE); // no movement yet

  // Arm setup
  // 
  arm_motor.run(FORWARD);
  delay(750);
  arm_motor.run(BACKWARD);
  delay(armDelay);
  arm_motor.run(RELEASE);

  // Slide setup
  slide_motor.run(BACKWARD);
  delay(750);
  slide_motor.run(RELEASE);
}
