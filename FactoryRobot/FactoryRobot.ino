#include "src/AFMotor/AFMotor.h"
#include "src/TimedAction/TimedAction.h"
#include "src/Adafruit_TCS34725/Adafruit_TCS34725.h"
#include "src/DHT/dht.h"

#define WATER_PIN  3

#define SIGNAL_WATER_PIN A1
#define SIGNAL_TEMP_PIN A0

// initialization of the motors
AF_DCMotor arm_motor(1);    // motor 1 is assigned to the arm
AF_DCMotor slide_motor(2);  // motor 2 is assigned to the slide

// initialization of the temperatur / humidity sensor
dht DHT;

// Sensor error codes
int waterError = 5;
float tempError = 70.0;
float humidityError = 95.0;

// booleans for the motors
boolean armEnable;    // global boolean for whether the arm should move
boolean slideEnable;  // global boolean for whether the slide should move
boolean slideOn;

// directions of the motor, for referencing
int armDir;   // global variable for the direction of the arm {-1 means backward, 0 means no movement, 1 means forward}

// arm motor variables
const int armSpeed = 255;           // global variable for the rotating speed of the arm motor
const unsigned long armDelay = 180; // the time the arm has to move before switching directions
unsigned long armStartMillis = 0;   // the starting time of the armMove process, 0 means no process
const int armFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the armMove method

// slide motor variables
const int slideSpeed= 255;            // global variable for the rotating speed of the slide motor
const unsigned long slideDelay = 300; // the time the slide has to move before stopping          
const unsigned long slideWait = 400;  // the time the slide has to stay in place
unsigned long slideStartMillis = 0;   // the starting time of the slideMove process, 0 means no process
const int slideFreq = 1;              // the wait time in milliseconds before the TimedAction should try to refire the slideMove method

// sensor variables
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // the sensor variable
int acceptance = 100;// the euclidean distance acceptance variable (distance < acceptance)

int sensorDetectDelay = 900; // delay between sensing the disk and moving the arm / slide
int errorCounter = 0;
int greenCounter = 0;

/**
 * Our method for checking the value of the water sensor
 */
int waterCheck() {
  int value;
  digitalWrite(WATER_PIN, HIGH);        // turn the sensor ON
  delay(10);                            // wait 10 milliseconds
  value = analogRead(SIGNAL_WATER_PIN); // read the analog value from sensor
  digitalWrite(WATER_PIN, LOW);         // turn the sensor OFF
  return value;
}

/**
 * Our own shutdown method to shutdown the robot
 */
void shutDown(int code) {
  arm_motor.run(RELEASE);
  slide_motor.run(RELEASE);
  digitalWrite(WATER_PIN, LOW);
  exit(code);
}

/**
 * Method to print color data to serial
 */
void printColor(uint16_t r, uint16_t g, uint16_t b) {
  Serial.print("Color Data:  ");
  Serial.print("R: "); Serial.print(r); Serial.print(", ");
  Serial.print("G: "); Serial.print(g); Serial.print(", ");
  Serial.print("B: "); Serial.print(b); Serial.println();
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
      return; // stop method for quicker refire
    }
    else if (m-armStartMillis >= sensorDetectDelay) {
      if (m-armStartMillis >= armDelay+sensorDetectDelay) {  // if past first checkpoint
        if (m-armStartMillis >= 2*armDelay+sensorDetectDelay) { // if also past second checkpoint
          //Serial.println("ARM: Finished!");  // log finish in serial
          arm_motor.run(RELEASE); // stop movement
          armEnable = false;  // set enable value to zero
          armStartMillis = (unsigned long) 0; // reset global start millis value
          armDir = 0;
          return; // stop method
        }
        else if(armDir == 1) {  // if past first, not past second and direction is still forward
            arm_motor.run(BACKWARD);  // move backward
            //Serial.println("ARM: Pushing!");  // log backward movement in serial
            armDir = -1;  // set direction to -1 (backward)
            return; // stop method for quicker refire
        }
      }
      else if (armDir == 0) {
        arm_motor.run(FORWARD); // move arm forward
        //Serial.println("ARM: Pulling!");  // log the movement in serial
        armDir = 1; // set direction value to 1 (forward)
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
    if (!slideOn) {
      if (slideStartMillis == 0) {  // if the global start millis variable equals zero, the method is just starting
        slideStartMillis = m; // set new value for start millis
        slide_motor.run(FORWARD);  // make the motor run backward
        //Serial.println("SLIDE: Forward!");  // log start in serial
        return; // stop function for quicker refire
      }
      else if (m-slideStartMillis >= slideDelay) {  // if time is past first checkpoint
        slide_motor.run(RELEASE); // stop motor
        slideEnable = false;  // set function execution to false
        slideOn = true;
        slideStartMillis = 0; // reset start millis value
        //Serial.println("SLIDE: Finished!"); // log finish in serial
        return; // stop the function
      }
    }
    else {
      if (slideStartMillis == 0) {  // if the global start millis variable equals zero, the method is just starting
        slideStartMillis = m; // set new value for start millis
        slide_motor.run(BACKWARD);  // make the motor run backward
        //Serial.println("SLIDE: Backward!");  // log start in serial
        return; // stop function for quicker refire
      }
      else if (m-slideStartMillis >= slideDelay) {
        slide_motor.run(RELEASE); // stop motor
        slideEnable = false;  // set function execution to false
        slideOn = false;
        slideStartMillis = 0; // reset start millis value
        //Serial.println("SLIDE: Finished!"); // log finish in serial
        return; // stop the function
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
  Serial.println();
  Serial.println("Setting up the robot!");

  if (tcs.begin()) {
    Serial.println("Found the TCS3472 sensor!");
  } else {
    Serial.println("The TCS3472 color sensor cannot be found. \nPlease make sure you connected your sensors correctly. \nIf it is all connected, try replacing the wires.");
    //while (1);
  }

  armEnable = false;
  slideEnable = false;
  slideOn = false;
  startup();
  tcs.begin();

  pinMode(WATER_PIN, OUTPUT); // config the pin as output
  digitalWrite(WATER_PIN, LOW);
  
  Serial.println("Robot fully setup!");
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

  if(millis() % sensorDetectDelay == 0) {

    // Temp and Humidity sensor
    DHT.read11(SIGNAL_TEMP_PIN);
    
    int waterValue = waterCheck();
    
    if(waterValue > waterError) {
      Serial.print("ERROR: Water detected on the sensor! \nWater value: ");
      Serial.println(waterValue);
      delay(100);
      shutDown(1);
    }
  
    if((DHT.temperature > tempError) || (DHT.humidity > humidityError)) {
      Serial.print("ERROR: Temperature too hot for the arduino! \nTemperature: ");
      Serial.print(DHT.temperature);
      Serial.print(", Humidity: ");
      Serial.println(DHT.humidity);
      delay(100);
      shutDown(1);
    }
  
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);
  
    color = getColor(r, g, b);
    //printColor(r, g, b); // disable to not show color data
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
      Serial.println("DISK DETECTION: White!");
    break;
    case 2:
      slideEnable = true;
      delay(sensorDetectDelay);
      armEnable = true;
      errorCounter = 0;
      Serial.println("DISK DETECTION: Black!");
    break;
    case 3:
      delay(sensorDetectDelay);
      greenCounter++;
      Serial.println("DISK DETECTION: Green!");
      Serial.print("Total of greens: "); Serial.println(greenCounter);
      errorCounter = 0;
    break;
    case -1:
      errorCounter++;
    break;
    }

    if(errorCounter > 1 && errorCounter < 5) {
      Serial.println("ERROR: Color mismatch with our database.");
      delay(10);
    } else if(errorCounter >= 5) {
      Serial.println("ERROR: Something stuck on the belt, please remove this object! \nRobot is now in standby mode.");
      delay(7000);
      Serial.println("Robot starting back up in 3 seconds!");
      delay(3000);
    }
  }
  
  
}
