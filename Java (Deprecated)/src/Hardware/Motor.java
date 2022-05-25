package Hardware;
import org.sintef.jarduino.*;
import org.sintef.jarduino.comm.Serial4JArduino;

/**
 * The class to monitor and control the motors used by our robot. This will be used for both the motors in the arms and in the belt.
 * If necessary, we will create subclasses that will be devided between the arm motors and the belt motors.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Motor extends JArduino {
	final private int POWER_PIN_NUMBER;
	final private PWMPin POWER_PIN;
	final private int DIRECTION_PIN_NUMBER;
	final private DigitalPin DIRECTION_PIN;
	final private int COUNTER_DIRECTION_PIN_NUMBER;
	final private DigitalPin COUNTER_DIRECTION_PIN;
	private int direction = 0;	// 0 means clockwise, 1 means counterclockwise

	/**
	 * The initialization of the motor class.
	 */
	public Motor(String port , int powerPinNumber, int directionPinNumber, int counterDirectionPinNumber) {
		super(port);
		this.POWER_PIN_NUMBER = powerPinNumber;
		this.POWER_PIN = PWMPin.fromValue((byte) POWER_PIN_NUMBER);
		this.DIRECTION_PIN_NUMBER = directionPinNumber;
		this.DIRECTION_PIN = DigitalPin.fromValue((byte) DIRECTION_PIN_NUMBER);
		this.COUNTER_DIRECTION_PIN_NUMBER = counterDirectionPinNumber;
		this.COUNTER_DIRECTION_PIN = DigitalPin.fromValue((byte) COUNTER_DIRECTION_PIN_NUMBER);
	}

	public Motor(String port, int powerPinNumber) {
		super(port);
		this.POWER_PIN_NUMBER = powerPinNumber;
		this.POWER_PIN = PWMPin.fromValue((byte) POWER_PIN_NUMBER);
		this.DIRECTION_PIN_NUMBER = -1;
		this.DIRECTION_PIN = null;
		this.COUNTER_DIRECTION_PIN_NUMBER = -1;
		this.COUNTER_DIRECTION_PIN = null;
	}

	public void setup() {
		if (!(DIRECTION_PIN == null) && !(COUNTER_DIRECTION_PIN == null)) {
			pinMode(DIRECTION_PIN, PinMode.OUTPUT);
			pinMode(COUNTER_DIRECTION_PIN, PinMode.OUTPUT);
		}
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
		analogWrite(POWER_PIN, (byte) 255);
	}
	
	@Override
	public void stopArduinoProcess() {
		analogWrite(POWER_PIN, (byte) 0);
		super.stopArduinoProcess();
	}

	public void setClockWise() {
		if (!(DIRECTION_PIN == null) && !(COUNTER_DIRECTION_PIN == null)) {
			direction = 0;
			digitalWrite(DIRECTION_PIN, DigitalState.HIGH);
			digitalWrite(COUNTER_DIRECTION_PIN, DigitalState.LOW);
		}
	}

	public void setCounterClockWise() {
		if (!(DIRECTION_PIN == null) && !(COUNTER_DIRECTION_PIN == null)) {
			direction = 1;
			digitalWrite(DIRECTION_PIN, DigitalState.LOW);
			digitalWrite(COUNTER_DIRECTION_PIN, DigitalState.HIGH);
		}
	}

	public int getDirection() {
		return direction;
	}

	public void grabDisk() {
        this.setClockWise();
        this.runArduinoProcess();
        delay(3000);
        this.stopArduinoProcess();
        this.setCounterClockWise();
        this.runArduinoProcess();
        delay(3000);
        this.stopArduinoProcess();
    }

	public static void main(String[] args) {
		String serialPort;
        if (args.length == 1) {
            serialPort = args[0];
        } else {
            serialPort = Serial4JArduino.selectSerialPort();
        }
		Motor arm = new Motor(serialPort, 11,2,3);
		arm.grabDisk();
	}
}
