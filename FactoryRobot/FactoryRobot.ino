#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"

AF_DCMotor arm_motor(2);
AF_DCMotor slide_motor(1);
boolean armEnable;
boolean slideEnable;
const int armSpeed = 255;
const int armDelay = 255;
const int armFreq = 500;
const int slideSpeed= 255;
const int slideDelay = 300;

void startup() {
  // turn on motor
  arm_motor.setSpeed(armSpeed);
  arm_motor.run(RELEASE);

  // turn on motor
  slide_motor.setSpeed(slideSpeed);
  slide_motor.run(RELEASE);
  
  arm_motor.run(FORWARD);
  delay(750);
  arm_motor.run(BACKWARD);
  delay(armDelay);
  arm_motor.run(RELEASE);
}

void armMove() {
  if (armEnable) {
    arm_motor.run(FORWARD);
    delay(armDelay);
    arm_motor.run(BACKWARD);
    delay(armDelay);
    arm_motor.run(RELEASE);
    Serial.println("Arm moving!");
    armEnable = false;
  }
}

void slideMove() {
  if (slideEnable) {
    slide_motor.run(BACKWARD);
    delay(slideDelay);
    slide_motor.run(RELEASE);
    delay(700);
    slide_motor.run(FORWARD);
    delay(slideDelay - 7);
    slide_motor.run(RELEASE);
    Serial.println("Slide moving!");
    slideEnable = false;
  }
}

TimedAction armThread = TimedAction(armFreq,armMove);
TimedAction slideThread = TimedAction(slideFreq,slideMove);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Setup!");

  armEnable = true;
  slideEnable = true;
  startup();
  Serial.println("startup complete!");
  delay(1000);
}

void loop() {
  armThread.check();
  slideThread.check();
  if(armEnable) {
    slideMove();
    armEnable = false;
  }

  delay(1000);
  armEnable = true;
}
