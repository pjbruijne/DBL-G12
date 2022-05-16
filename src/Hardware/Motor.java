package Hardware;
import org.sintef.jarduino.*;

/**
 * The class to monitor and control the motors used by our robot. This will be used for both the motors in the arms and in the belt.
 * If necessary, we will create subclasses that will be devided between the arm motors and the belt motors.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Motor extends JArduino {
	final private int PIN_NUMBER;
	final private DigitalPin PIN;

	/**
	 * The initialization of the motor class.
	 */
	public Motor(String port ,int pinNumber) {
		super(port);
		this.PIN_NUMBER = pinNumber;
		this.PIN = DigitalPin.fromValue((byte) PIN_NUMBER);
	}

	public void setup() {
		pinMode(PIN, PinMode.OUTPUT);
	}

	public void loop() {
		try {
			// TODO: loop structure motors
		} catch (Exception e) {
			// TODO: Exception handling
		} catch (Error e) {
			// TODO: Error handling
		}
	}

	@Override
	public void runArduinoProcess() {
		super.runArduinoProcess();
		digitalWrite(PIN, DigitalState.HIGH);
	}
	
	@Override
	public void stopArduinoProcess() {
		digitalWrite(PIN, DigitalState.LOW);
		super.stopArduinoProcess();
	}
}
