import org.sintef.jarduino.*;
import org.sintef.jarduino.comm.Serial4JArduino;

import EmergencyState.*;
import ErrorHandling.*;
import Hardware.*;
/**
 * The main class for our robot that will automate the entire robot.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Main {
    //The global variables and constants we will be using during our program runs.

    //The variables for the arduino
    final private String PORT;
    //The variables for the belt motor
    final int BELT_MOTOR_PIN_NUMBER = 1;
    final Motor BELT_MOTOR;
    //The variables for the arm motor
    final int ARM_MOTOR_PIN_NUMBER = 2;
    final int ARM_MOTOR_CLOCK_PIN_NUMBER = 3;
    final int ARM_MOTOR_COUNTER_CLOCK_PIN_NUMBER = 4;
    final Motor ARM_MOTOR;
    final int GRAB_TIME = 1500;
    //The variables for the sensors
    final int INFRARED_SENSOR_PIN_NUMBER = 6;
    final int COLOR_SENSOR_PIN_NUMBER = 7;
    final Sensor SENSOR;
    //The variables for the emergency light
    final int EMERGENCY_PIN_NUMBER = 13;
    final int EMERGENCY_BLINK_SPEED = 500;
    final LEDBlink EMERGENCY_LIGHT;
    boolean emergencyState = false;

    /**
     * The initialization method of our main class. It will be used to create certain objects at the moment of creation.
     */
    public Main(String port) {
        PORT = port;
        ARM_MOTOR = new Motor(PORT, ARM_MOTOR_PIN_NUMBER, ARM_MOTOR_CLOCK_PIN_NUMBER, ARM_MOTOR_COUNTER_CLOCK_PIN_NUMBER);
        BELT_MOTOR = new Motor(PORT, BELT_MOTOR_PIN_NUMBER);
        SENSOR = new Sensor(PORT, INFRARED_SENSOR_PIN_NUMBER, COLOR_SENSOR_PIN_NUMBER);
        EMERGENCY_LIGHT = new LEDBlink(PORT,EMERGENCY_PIN_NUMBER, EMERGENCY_BLINK_SPEED);
    }


    /**
     * The method to be run by the Main class. Everything the program will have to do will be handled by this function
     */
    public void run() {
        try {
            // TODO: main run statements
            grabDisk(ARM_MOTOR);

        } //catch (Error e) {
            // toggleEmergencyState();
            // wait(10000);
        //}
        catch (Exception e) {
            System.out.println(e);
        }

    }

    /**
     * The toggle switch for the emergency state. In emergency state the emergency LED will start blinking periodically.
     */
    void toggleEmergencyState() {
        try {
            if (!emergencyState) {      // if there is no emergency before toggling
                EMERGENCY_LIGHT.runArduinoProcess();    // run process to make led blink
                emergencyState = true;  // set the emergency state to true
            }
            else {                      // if the emergency state is already on before toggling
                EMERGENCY_LIGHT.stopArduinoProcess(); // stop blinking process
                emergencyState = false; // set the emergency state to false
            }
        } catch (Exception e) {
            // TODO: toggleEmergencyState exception handling
        } catch (Error e) {
            // TODO: toggleEmergencyState error handling
        }
    }

    void grabDisk(Motor motor) {
        motor.setClockWise();
        motor.runArduinoProcess();
        wait(GRAB_TIME);
        motor.stopArduinoProcess();
        motor.setCounterClockWise();
        motor.runArduinoProcess();
        wait(GRAB_TIME);
        motor.stopArduinoProcess();
    }

    /**
     * A method for quickly issuing a sleep in the thread without having to deal with the exception all the time.
     * @param waitTime The time to wait for the thread to resume
     */
    public void wait(int waitTime) {
        try {
            Thread.sleep(waitTime);
        }
        catch (InterruptedException e) {
            System.out.println("The current thread got interrupted");
            Thread.currentThread().interrupt();
        }
    }

    /**
     * The method called by the main thread. It first prints out the exiting hotkey in the print stream before calling the run function.
     * @param args
     */
    public static void main(String[] args) {
        String serialPort;
        if (args.length == 1) {
            serialPort = args[0];
        } else {
            serialPort = Serial4JArduino.selectSerialPort();
        }
        (new Main(serialPort)).run();
    }

}
