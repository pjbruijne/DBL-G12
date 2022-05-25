#include <AFMotor.h>

AF_DCMotor arm_motor(2);
AF_DCMotor slide_motor(1);
boolean armEnable;
const int armDelay = 255;
const int slideDelay = 300;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Setup!");

  armEnable = true;
  startup();
  Serial.println("startup complete!");
  delay(1000);
}

void loop() {
  if(armEnable) {
    slideMove();
    armEnable = false;
  }

  delay(1000);
  armEnable = true;
}

void startup() {
  // turn on motor
  arm_motor.setSpeed(255);
  arm_motor.run(RELEASE);

  // turn on motor
  slide_motor.setSpeed(255);
  slide_motor.run(RELEASE);
  
  arm_motor.run(FORWARD);
  delay(750);
  arm_motor.run(BACKWARD);
  delay(armDelay);
  arm_motor.run(RELEASE);
}

void armMove() {
  arm_motor.run(FORWARD);
  delay(armDelay);
  arm_motor.run(BACKWARD);
  delay(armDelay);
  arm_motor.run(RELEASE);
  Serial.println("Arm moving!");
}

void slideMove() {
  slide_motor.run(BACKWARD);
  delay(slideDelay);
  slide_motor.run(RELEASE);
  delay(700);
  slide_motor.run(FORWARD);
  delay(slideDelay - 7);
  slide_motor.run(RELEASE);
  Serial.println("Slide moving!");
}
