package Hardware;

import java.io.IOException;

import ErrorHandling.Errors.SignalError;

/**
 * The class to monitor and control the motors used by our robot. This will be used for both the motors in the arms and in the belt.
 * If necessary, we will create subclasses that will be devided between the arm motors and the belt motors.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Motor implements Runnable {
	final private int PIN_NUMBER;
	volatile boolean running;

	/**
	 * The initialization of the motor class.
	 */
	public Motor(int pinNumber) {
		this.PIN_NUMBER = pinNumber;
	}

	public void run() {
		try {
			System.out.println("The motor on pin "+PIN_NUMBER+" has started running");
			running = true;
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode "+PIN_NUMBER+" out");
			do {
				runtime.exec("gpio write "+PIN_NUMBER+" 1");
			}
			while(running);
			runtime.exec("gpio write "+PIN_NUMBER+" 0");
			System.out.println("The motor on pin "+PIN_NUMBER+" has stopped running");
		}
		catch (InterruptedException e) {
			System.out.println("The motor on pin "+PIN_NUMBER+" got interrupted");
            Thread.currentThread().interrupt();
		}
		catch (IOException e) {
			throw new SignalError("Signal Error: The motor on pin "+PIN_NUMBER+" lost connection to the Raspberry Pi.");
		}
		catch (Exception e) {
			System.out.println(e);
		}
	}

	public void stop() {
		running = false;
	}
}
