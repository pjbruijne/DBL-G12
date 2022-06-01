#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"
#include "src/Adafruit_TCS34725/Adafruit_TCS34725.h"

AF_DCMotor arm_motor(1);
AF_DCMotor slide_motor(2);

boolean armEnable;
boolean slideEnable;

int armDir;
int slideDir;

const int armSpeed = 255;
const unsigned long armDelay = 220;
const int armFreq = 1;
unsigned long armStartMillis = 0;

const int slideSpeed= 255;
const unsigned long slideDelay = 300;
const int slideFreq = 1;
const unsigned long slideWait = 700;
unsigned long slideStartMillis = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
int acceptance = 70;

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

  // Slide setup
  slide_motor.run(BACKWARD);
  delay(750);
  slide_motor.run(FORWARD);
  delay(100);
  slide_motor.run(RELEASE);
}

void armMove() {
  if (armEnable) {
    unsigned long m = millis();
    if (armStartMillis == (unsigned long) 0) {
      armStartMillis = m;
      Serial.println(m);
      arm_motor.run(FORWARD);
      Serial.println("Arm starts pulling!");
      armDir = 1;
      return;
    }
    else if (m-armStartMillis >= armDelay) {
      if (m-armStartMillis >= 2*armDelay) {
        Serial.println("Arm has finished!");
        arm_motor.run(RELEASE);
        armEnable = false;
        armStartMillis = (unsigned long) 0;
        return;
      }
      else if(armDir == 1) {
          arm_motor.run(BACKWARD);
          Serial.println("Arm starts moving back!");
          armDir = -1;
          return;
      }
    }
  }
}

/**
 * Our method for moving the slide. Since it is in a TimedAction, it will constantly be fired. Therefore we first always check whether the slide is supposed to move.
 */
void slideMove() {
  if (slideEnable) {  // if the slide is supposed to move
    unsigned long m = millis(); //take time value since start of machine
    if (slideStartMillis == 0) {  // if the global start millis variable equals zero, the method is just starting
      slideStartMillis = m; // set new value for start millis
      slide_motor.run(FORWARD);  // make the motor run backward
      slideDir = -1;  // set direction to -1 (backward)
      Serial.println("Slide started");
      return;
    }
    else if (m-slideStartMillis >= slideDelay) {  // if time is past first checkpoint
      if (m-slideStartMillis >= slideDelay+slideWait) { // if time is past second checkpoint
        if (millis() - slideStartMillis >= slideDelay*2+slideWait-7) {  //if time is past third checkpoint
          slide_motor.run(RELEASE); // stop motor
          slideEnable = false;  // set function execution to false
          slideStartMillis = 0; // reset start millis value
          Serial.println("Slide checkpoint 4");
          return; // stop the function
        }
        else if (slideDir == 0) {  // if past second checkpoint, not the third yet and not moving
          slide_motor.run(BACKWARD); // run forward
          slideDir = 1; // set direction to 1 (forward)
          Serial.println("Slide checkpoint 3");
          return;
        }
      }
      else if (slideDir == -1) {  // if past first checkpoint but not yet the second and also moving backward
        slide_motor.run(RELEASE); // stop movement
        slideDir = 0;  // set direction variable to 0
        Serial.println("Slide checkpoint 2");
        return;
      }
    }
    
  }
}

int getColor(float r, float g, float b) {
  float black[] = {0,0,0};
  float green[] = {0,255,0};
  float white[] = {255,255,255};
  float blackDist = sqrt(sq(black[0]-r)+sq(black[1]-g)+sq(black[2]-b));
  float greenDist = sqrt(sq(green[0]-r)+sq(green[1]-g)+sq(green[2]-b));
  float whiteDist = sq(sq(white[0]-r)+sq(white[1]-g)+sq(white[2]-b));
  if (whiteDist < acceptance) {
    return 0;
  }
  else if (blackDist < acceptance) {
    return 1;
  }
  else if (greenDist < acceptance) {
    return 2;
  }
  else return -1;
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
  if(millis() % 1000 == 0) {
    armEnable = true;
    slideEnable = true;
  }
  if (millis()>10000) {
    arm_motor.run(RELEASE);
    slide_motor.run(RELEASE);
    exit(0);
  }
}
