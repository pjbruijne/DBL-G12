import EmergencyState.*;
import ErrorHandling.*;
import Hardware.Sensor;

/**
 * The main class for our robot that will automate the entire robot.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Main {
    //The global variables and constants we will be using during our program runs.

    //The variables for the motors
    final int BELT_MOTOR_PIN_NUMBER = 1;
    final int ARM_MOTOR_PIN_NUMBER = 2;
    //The variables for the sensors
    final int INFRARED_SENSOR_PIN_NUMBER = 3;
    final int COLOR_SENSOR_PIN_NUMBER = 5;
    final Sensor SENSOR;
    Thread sensorThread;
    //The variables for the emergency light
    final int EMERGENCY_PIN_NUMBER = 4;
    final int EMERGENCY_BLINK_SPEED = 500;
    final LEDBlink EMERGENCY_LIGHT;
    boolean emergencyState = false;
    Thread emergencyThread;

    /**
     * The initialization method of our main class. It will be used to create certain objects at the moment of creation.
     */
    public Main() {
        EMERGENCY_LIGHT = new LEDBlink(EMERGENCY_PIN_NUMBER, EMERGENCY_PIN_NUMBER);

    }

    /**
     * The method to be run by the Main class. Everything the program will have to do will be handled by this function
     */
    public void run() {
        try {
            LEDBlink ledBlink = new LEDBlink(EMERGENCY_PIN_NUMBER, EMERGENCY_BLINK_SPEED);
            Thread ledBlinkThread = new Thread(ledBlink);
            ledBlinkThread.start();
            wait(10000);
            ledBlink.stop();
            ledBlinkThread.join();
            // TODO: we need to create threads for the motors and the sensors and call them.
        }
        catch (Error e) {
            toggleEmergencyState();
            wait(10000);

        }
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
                Thread emergencyThread = new Thread(EMERGENCY_LIGHT); // create thread for emergency light to blink
                emergencyThread.start();
                emergencyState = true;  // set the emergency state to true
            }
            else {                      // if the emergency state is already on before toggling
                EMERGENCY_LIGHT.stop(); // stop the blinking
                emergencyThread.join(); // wait for the emergency light thread to die
                emergencyState = false; // set the emergency state to false
            }
        }
        catch (InterruptedException e) {
            System.out.println("The current thread got interrupted");
            Thread.currentThread().interrupt();
        }
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
        System.out.println("Press CTRL-C to exit");
        (new Main()).run();
    }

}
