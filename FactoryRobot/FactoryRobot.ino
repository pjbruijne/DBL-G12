#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"
#include "src/Adafruit_TCS34725/Adafruit_TCS34725.h"

AF_DCMotor arm_motor(1);    // motor 1 is assigned to the arm
AF_DCMotor slide_motor(2);  // motor 2 is assigned to the slide

boolean armEnable;    // global boolean for whether the arm should move
boolean slideEnable;  // global boolean for whether the slide should move

int armDir;   // global variable for the direction of the arm {-1 means backward, 0 means no movement, 1 means forward}
int slideDir; // global variable for the direction of the slide {-1 means backward, 0 means no movement, 1 means forward}

const int armSpeed = 255;           // global variable for the rotating speed of the arm motor
const unsigned long armDelay = 220; // the time the arm has to move before switching directions
unsigned long armStartMillis = 0;   // the starting time of the armMove process, 0 means no process
const int armFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the armMove method

const int slideSpeed= 255;            // global variable for the rotating speed of the slide motor
const unsigned long slideDelay = 300; // the time the slide has to move before stopping          
const unsigned long slideWait = 700;  // the time the slide has to stay in place
unsigned long slideStartMillis = 0;   // the starting time of the slideMove process, 0 means no process
const int slideFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the slideMove method

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // the sensor variable
int acceptance = 150;                                                                        // the euclidean distance acceptance variable (distance < acceptance)

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
  slide_motor.run(FORWARD);
  delay(100);
  slide_motor.run(RELEASE);
}

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

/**
 * Distance calculated using the p-norm. Used by our function below to calculate the distance between the raw data and the preset colors.
 */
float pNormDistance(uint16_t vector1[], uint16_t vector2[], int p) {
  float x = 0;
  int len1 = sizeof(vector1)/sizeof(vector1[0]);
  int len2 = sizeof(vector2)/sizeof(vector2[0]);
  if (len1 != len2) {
    return -1;
  }
  for (int i = 0; i < len1; i++) {
    x+= pow(abs(((float)vector1[i])-((float)vector2[i])),p);
  }
  return pow(x,1/p);
}

/**
 * Our method for getting the disk color based on the rgb values provided by the sensor.
 */
int getColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c, uint16_t lux) {
  int p = 2;  // we use euclidean distance, but we could easily switch to manhattan or other set distances
  uint16_t rawData[] = {r,g,b,c,lux}; // the rgb c and lux values of input in an array
  uint16_t black[] = {115,120,100,0,80};  // the values of black 2
  uint16_t green[] = {310,600,350,0,500};  // the values of green 3
  uint16_t white[] = {1300,1750,1620,0,1000};  // the values of white 1
  uint16_t belt[] = {0,52,33,0,0}; // the values of the belt 0
  float blackDist = pNormDistance(rawData,black,p); // the euclidean distance between the sensor rgb and black
  float greenDist = pNormDistance(rawData,green,p); // the euclidean distance between the sensor rgb and green
  float whiteDist = pNormDistance(rawData,white,p); // the euclidean distance between the sensor rgb and white
  float beltDist = pNormDistance(rawData,belt,p);  // the euclidean distance between the sensor rgb and belt
  float minDist = min(min(beltDist,blackDist),min(greenDist,whiteDist));
  if (minDist < acceptance) { // if there is a distance smaller than the acceptance variable
    if (whiteDist == minDist) {  // if white has smallest distance to raw data
      return 1; // disk is white
    }
    else if (blackDist == minDist) {  // if black has smallest distance to raw data 
      return 2; // disk is black
    }
    else if (greenDist == minDist) {  // if green has smallest distance to raw data
      return 3; // disk is green
    }
    else return 0;  // else belt has smallest distance to raw data by process of elimination, no disk present
  }
  else return -1; // if smallest distance is not smaller than acceptance, return -1 meaning something wrong is on the belt
}

TimedAction armThread = TimedAction(armFreq,armMove);       // TimedAction of the armMove function for protothreading
TimedAction slideThread = TimedAction(slideFreq,slideMove); // TimedAction of the slideMove function for protothreading

/**
 * Our setup method, which is the first method to be executed when starting the program. Certain variables are initialized here.
 */
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Setup!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
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

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  int color = getColor(r, g, b, c, lux);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" : ");
  Serial.println(color);
  
  armThread.check();  // check if the armThread has to be refired yet
  slideThread.check();  // check if the slideThread has to be refired yet
  if(millis() % 1000 == 0) {  // for testing purposes
    armEnable = true;
    slideEnable = true;
  }
  if (millis()>10000) { // early exit call for testing purposes
    arm_motor.run(RELEASE);
    slide_motor.run(RELEASE);
    exit(0);
  }
}
