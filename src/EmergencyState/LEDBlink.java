package EmergencyState;
import org.sintef.jarduino.*;

/**
 * The class to oversee the emergency light. It will be used in a seperate thread to allow for multiple routines happening simultaneously.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class LEDBlink extends JArduino {

    final private int PIN_NUMBER;   // WiringPin diagram
    final private int BLINK_SPEED;  // The time the light stays on/off in miliseconds
    final private DigitalPin PIN;   // The pin variable to be initialized with the pin number

    // Constructor
    public LEDBlink(String port ,int pinNumber, int blinkSpeed) {
        super(port);
        this.PIN_NUMBER = pinNumber;
        this.BLINK_SPEED = blinkSpeed;
        this.PIN = DigitalPin.fromValue((byte) PIN_NUMBER);
    }

    public void setup() {
        pinMode(PIN, PinMode.OUTPUT);
    }

    public void loop() {
        try {
            digitalWrite(PIN, DigitalState.HIGH); 
            delay(BLINK_SPEED);
            digitalWrite(PIN, DigitalState.LOW);
            delay(BLINK_SPEED);
        } catch (Exception e) {
            // TODO: Exception handling
            System.out.println("Exception occured: " + e.getMessage());
        } catch (Error e) {
            // TODO: Error handling
        }
    }

}