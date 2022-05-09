import EmergencyState.*;
import ErrorHandling.*;

public class Main {
    //The pin numbers we use for certain parts of the robot and other necessary values
    final int motorPin = 5;

    final int emergencyPin = 4;
    final int emergencyBlinkSpeed = 500;
    boolean emergencyState = false;
    LEDBlink emergencyLight;
    Thread emergencyThread;


    public Main() {
        emergencyLight = new LEDBlink(emergencyPin, emergencyBlinkSpeed);
    }

    public void run() {
        try {
            LEDBlink ledBlink = new LEDBlink(4, 500);
            Thread ledBlinkThread = new Thread(ledBlink);
            ledBlinkThread.start();
            Thread.sleep(10000);
            ledBlink.stop();
            ledBlinkThread.join();
        }
        catch (Exception | Error e) {

        }

    }

    void toggleEmergencyState() {
        try {
            if (!emergencyState) {  // if there is no emergency before toggling
                Thread emergencyThread = new Thread(emergencyLight); // create thread for emergency light to blink
                emergencyThread.start();
                emergencyState = true;  // set the emergency state to true
            }
            else {  // if the emergency state is already on before toggling
                emergencyLight.stop();  // stop the blinking
                emergencyThread.join();
                emergencyState = false; // set the emergency state to false
            }
        }
        catch (InterruptedException e) {

        }
    }

    public static void main(String[] args) {
        // Just a blinking led effect
        System.out.println("Press CTRL-C to exit");
        (new Main()).run();
    }

}
