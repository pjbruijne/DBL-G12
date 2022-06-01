#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"

AF_DCMotor arm_motor(1);
AF_DCMotor slide_motor(2);

boolean armEnable;
boolean slideEnable;

const int armSpeed = 255;
const unsigned long armDelay = 255;
const int armFreq = 1;
unsigned long armStartMillis = 0;

const int slideSpeed= 255;
const unsigned long slideDelay = 300;
const int slideFreq = 1;
const unsigned long slideWait = 700;
unsigned long slideStartMillis = 0;

int millisError = 100;

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
    unsigned long m = millis();
    if (armStartMillis == (unsigned long) 0) {
      armStartMillis = m;
      Serial.println(m);
      arm_motor.run(FORWARD);
      Serial.println("Arm starts pulling!");
    }
    else if (m-armStartMillis < armDelay) {
      
    }
    else if (m-armStartMillis >= 2*armDelay) {
      Serial.println("Arm has finished!");
      arm_motor.run(RELEASE);
      armEnable = false;
      armStartMillis = (unsigned long) 0;
      return;
    }   
    else if(arm_motor.) {
      arm_motor.run(BACKWARD);
      Serial.println("Arm starts moving back!");
    }
  }
}

void slideMove() {
  if (slideEnable) {
    unsigned long m = millis();
    if (slideStartMillis == 0) {
      slideStartMillis = m;
      slide_motor.run(BACKWARD);
    }
    else if (m-slideStartMillis >= slideDelay) {
      if (millis()-slideStartMillis == slideDelay+slideWait) {
      slide_motor.run(FORWARD);
      }
      else if (millis() - slideStartMillis >= slideDelay*2+slideWait-7) {
        slide_motor.run(RELEASE);
        slideEnable = false;
        slideStartMillis = 0;
        return;
      }
      else {
        slide_motor.run(RELEASE);
      }
    }
    
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
  armEnable = true;
  slideEnable = true;
  if (millis()>10000) {
    exit(0);
  }
}
