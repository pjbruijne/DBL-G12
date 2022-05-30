#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"

AF_DCMotor arm_motor(1);
AF_DCMotor slide_motor(2);

boolean armEnable;
boolean slideEnable;

const int armSpeed = 255;
const int armDelay = 255;
const int armFreq = 10;
int armCounter = 0;

const int slideSpeed= 255;
const int slideDelay = 300;
const int slideFreq = 10;
const int slideWait = 700;
int slideCounter = 0;

void startup() {
  // turn on motor
  arm_motor.setSpeed(armSpeed);
  arm_motor.run(RELEASE);

  // turn on motor
  slide_motor.setSpeed(slideSpeed);
  slide_motor.run(RELEASE);

  // Arm setup
  arm_motor.run(FORWARD);
  delay(750);
  arm_motor.run(BACKWARD);
  delay(armDelay);
  arm_motor.run(RELEASE);
}

void armMove() {
  if (armEnable) {
    if (armCounter == 0) {
      arm_motor.run(FORWARD);
      Serial.println("Arm starts pulling!");
    }
    else if (abs(armCounter-armDelay) < armFreq) {
      arm_motor.run(BACKWARD);
      Serial.println("Arm starts moving back!");
    }
    else if (abs(armCounter-2*armDelay) < armFreq) {
      Serial.println("Arm has finished!");
      arm_motor.run(RELEASE);
      armEnable = false;
      armCounter = 0;
      return;
    }
    armCounter += armFreq;    
  }
}

void slideMove() {
  if (slideEnable) {
    if (slideCounter == 0) {
      slide_motor.run(BACKWARD);
    }
    else if (abs(slideCounter-slideDelay) < slideFreq) {
      slide_motor.run(RELEASE);
    }
    else if (abs(slideCounter-(slideDelay+slideWait)) < slideFreq) {
      slide_motor.run(FORWARD);
    }
    else if (slideCounter >= slideDelay*2+slideWait-7) {
      slide_motor.run(RELEASE);
      slideEnable = false;
      slideCounter = 0;
      return;
    }
    slideCounter += slideFreq;    
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
  delay(1000);
  armEnable = true;
  slideEnable = true;
}
