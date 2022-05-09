import EmergencyState.*;
import ErrorHandling.*;

public class Main {
    //The pin numbers we use for certain parts of the robot and other necessary values
    int motorPin = 5;

    int emergencyPin = 4;
    int emergencyBlinkSpeed = 500;
    boolean emergencyState = false;
    LEDBlink emergencyLight;


    public Main() {
        emergencyLight = new LEDBlink(emergencyPin, emergencyBlinkSpeed);
    }

    void toggleEmergencyState() {
        if (!emergencyState) {  // if there is no emergency before toggling
            emergencyLight.blink(); // start blinking the emergency light
            emergencyState = true;  // set the emergency state to true
        }
        else {  // if the emergency state is already on before toggling
            emergencyLight.stop();  // stop the blinking
            emergencyState = false; // set the emergency state to false
        }
    }

    public static void main(String[] args) {
        // Just a blinking led effect
        System.out.println("Press CTRL-C to exit");
        try {

        }
        catch (Error e) {
            LEDBlink ledBlink = new LEDBlink(4, 500);
        }
        LEDBlink ledBlink = new LEDBlink(4, 500);
        ledBlink.blink();
    }

}
