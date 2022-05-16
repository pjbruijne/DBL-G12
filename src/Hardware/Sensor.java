package Hardware;

import java.util.LinkedList;
import org.sintef.jarduino.*;

/** 
 * The class that will oversee the sensors and their input.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Sensor extends JArduino {
	final private int PIN_NUMBER_IR;
	final private int PIN_NUMBER_COLOR;
	final private DigitalPin PIN_IR;
	final private DigitalPin PIN_COLOR;
	volatile LinkedList<Disk> diskList;

	public Sensor(String port, int pinNumberInfrared, int pinNumberColor) {
		super(port);
		this.PIN_NUMBER_IR = pinNumberInfrared;
		this.PIN_NUMBER_COLOR = pinNumberColor;
		PIN_IR = DigitalPin.fromValue((byte) PIN_NUMBER_IR);
		PIN_COLOR = DigitalPin.fromValue((byte) PIN_NUMBER_COLOR);
		diskList = new LinkedList<Disk>();
	}

	public void setup() {
		pinMode(PIN_IR, PinMode.INPUT);
		pinMode(PIN_COLOR, PinMode.INPUT);
	}

	public void loop() {
		try {
			// TODO: create PIN_IR input var
			// TODO: create PIN_COLOR input var
			// if (condition) {
			// TODO: create input var reactive code
			// }
		} catch (Exception e) {
			// TODO: Exception handling
		} catch (Error e) {
			// TODO: Error handling
		}
	}
}