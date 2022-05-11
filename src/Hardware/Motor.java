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
	volatile boolean running = true;

	/**
	 * The initialization of the motor class.
	 */
	public Motor(int pinNumber) {
		this.PIN_NUMBER = pinNumber;
	}

	public void run() {
		try {
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode "+PIN_NUMBER+" out");
			do {
				runtime.exec("gpio write "+PIN_NUMBER+" 1");
			}
			while(running);
			runtime.exec("gpio write "+PIN_NUMBER+" 0");
		}
		catch (InterruptedException e) {
			System.out.println("One of the motor threads got interrupted");
            Thread.currentThread().interrupt();
		}
		catch (IOException e) {
			throw new SignalError();
		}
		catch (Exception e) {

		}
	}

	public void stop() {
		running = false;
	}
}
