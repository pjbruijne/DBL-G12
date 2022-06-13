#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"
#include "src/Adafruit_TCS34725/Adafruit_TCS34725.h"

// initialization of the motors
AF_DCMotor arm_motor(1);    // motor 1 is assigned to the arm
AF_DCMotor slide_motor(2);  // motor 2 is assigned to the slide

// booleans for the motors
boolean armEnable;    // global boolean for whether the arm should move
boolean slideEnable;  // global boolean for whether the slide should move

// directions of the motors, for referencing
int armDir;   // global variable for the direction of the arm {-1 means backward, 0 means no movement, 1 means forward}
int slideDir; // global variable for the direction of the slide {-1 means backward, 0 means no movement, 1 means forward}

// arm motor variables
const int armSpeed = 255;           // global variable for the rotating speed of the arm motor
const unsigned long armDelay = 180; // the time the arm has to move before switching directions
unsigned long armStartMillis = 0;   // the starting time of the armMove process, 0 means no process
const int armFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the armMove method

// slide motor variables
const int slideSpeed= 255;            // global variable for the rotating speed of the slide motor
const unsigned long slideDelay = 300; // the time the slide has to move before stopping          
const unsigned long slideWait = 700;  // the time the slide has to stay in place
unsigned long slideStartMillis = 0;   // the starting time of the slideMove process, 0 means no process
const int slideFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the slideMove method

// sensor variables
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // the sensor variable
int acceptance = 100;// the euclidean distance acceptance variable (distance < acceptance)

int checkInterval = 1100; // interval which the sensor checks in miliseconds
int sensorDetectDelay = 600; // delay between sensing the disk and moving the arm / slide
int errorCounter = 0;

/**
 * Our method for moving the arm. Since it is protothreaded, we constantly fire it and first check whether the arm should move.
 * Then we check whether it has passed certain checkpoints to see which action it should take.
 */
void armMove() {
  if (armEnable) {  // if the arm is supposed to move
    unsigned long m = millis(); // take time value since start of the program
    if (armStartMillis == (unsigned long) 0) {  // if global start millis value equals zero, the function has just been started
      armStartMillis = m; // set new value of global start millis variable
      arm_motor.run(FORWARD); // move arm forward
      Serial.println("Arm starts pulling!");  // log the movement in serial
      armDir = 1; // set direction value to 1 (forward)
      return; // stop method for quicker refire
    }
    else if (m-armStartMillis >= armDelay) {  // if past first checkpoint
      if (m-armStartMillis >= 2*armDelay) { // if also past second checkpoint
        Serial.println("Arm has finished!");  // log finish in serial
        arm_motor.run(RELEASE); // stop movement
        armEnable = false;  // set enable value to zero
        armStartMillis = (unsigned long) 0; // reset global start millis value
        return; // stop method
      }
      else if(armDir == 1) {  // if past first, not past second and direction is still forward
          arm_motor.run(BACKWARD);  // move backward
          Serial.println("Arm starts moving back!");  // log backward movement in serial
          armDir = -1;  // set direction to -1 (backward)
          return; // stop method for quicker refire
      }
    }
  }
}

/**
 * Our method for moving the slide. Since it is in a TimedAction, it will constantly be fired.
 * Therefore we first always check whether the slide is supposed to move.
 * Then we will see whether it has passed certain checkpoints to see which action it should take
 */
void slideMove() {
  if (slideEnable) {  // if the slide is supposed to move
    unsigned long m = millis(); //take time value since start of the program
    if (slideStartMillis == 0) {  // if the global start millis variable equals zero, the method is just starting
      slideStartMillis = m; // set new value for start millis
      slide_motor.run(FORWARD);  // make the motor run backward
      slideDir = -1;  // set direction to -1 (backward)
      Serial.println("Slide moves forward");  // log start in serial
      return; // stop function for quicker refire
    }
    else if (m-slideStartMillis >= slideDelay) {  // if time is past first checkpoint
      if (m-slideStartMillis >= slideDelay+slideWait) { // if time is past second checkpoint
        if (millis() - slideStartMillis >= slideDelay*2+slideWait-7) {  //if time is past third checkpoint
          slide_motor.run(RELEASE); // stop motor
          slideEnable = false;  // set function execution to false
          slideStartMillis = 0; // reset start millis value
          Serial.println("Slide has finished"); // log finish in serial
          return; // stop the function
        }
        else if (slideDir == 0) {  // if past second checkpoint, not the third yet and not moving
          slide_motor.run(BACKWARD); // run forward
          slideDir = 1; // set direction to 1 (forward)
          Serial.println("Slide moves back"); // log backward movement in serial
          return; // stop function for quicker refire
        }
      }
      else if (slideDir == -1) {  // if past first checkpoint but not yet the second and also moving backward
        slide_motor.run(RELEASE); // stop movement
        slideDir = 0;  // set direction variable to 0
        Serial.println("Slide stopped");  // log the stop in serial
        return; // stop function for quicker refire
      }
    }
    
  }
}

TimedAction armThread = TimedAction(armFreq,armMove);       // TimedAction of the armMove function for protothreading
TimedAction slideThread = TimedAction(slideFreq,slideMove); // TimedAction of the slideMove function for protothreading

/**
 * Our setup method, which is the first method to be executed when starting the program. Certain variables are initialized here.
 */
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println();Serial.println("Setup!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("The TCS34725 color sensor cannot be found. \nPlease make sure you connected your sensors correctly. \nIf it is all connected, try replacing the wires.");
    //while (1);
  }

  armEnable = true;
  slideEnable = true;
  startup();
  tcs.begin();
  Serial.println("startup complete!");
  delay(1000);
}

/**
 * The loop method. Constantly refired and mostly used for checking the protothreads and the rgb values of the sensor.
 */
void loop() {
  uint16_t r, g, b, c, colorTemp, lux;
  int color;
  
  armThread.check();  // check if the armThread has to be refired yet
  slideThread.check();  // check if the slideThread has to be refired yet

  if(millis() % checkInterval == 0) {
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);
  
    color = getColor(r, g, b);
  
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" : ");
    Serial.println(color);
    armThread.check();
    slideThread.check();

    if(color == 2 && !(slideEnable || armEnable)) {
      delay(sensorDetectDelay);
      slideEnable = true;
      armEnable = true;
    } else if(color == 1) {
      delay(sensorDetectDelay);
      armEnable = true;
    }

    switch(color){
    case 1:
      delay(sensorDetectDelay);
      armEnable = true;
      errorCounter = 0;
    break;
    case 2:
      delay(sensorDetectDelay);
      slideEnable = true;
      armEnable = true;
      errorCounter = 0;
    break;
    case 3:
      
    break;
    case -1:
      errorCounter++;
    break;
    }
  }
  
  if(errorCounter > 5) {
    Serial.println("There was an error with the color sensor. \nColor not recognized with the disks...");
    delay(100);
    exit(1);
  }
  
  
  if (millis()>60000) { // early exit call for testing purposes
    arm_motor.run(RELEASE);
    slide_motor.run(RELEASE);
    exit(0);
  }
}
