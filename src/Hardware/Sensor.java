package Hardware;

import java.util.LinkedList;

/** 
 * The class that will oversee the sensors and their input.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Sensor implements Runnable {
	final int PIN_NUMBER;
	private volatile boolean running;
	volatile LinkedList<Disk> diskList;

	public Sensor(int pinNumber) {
		this.PIN_NUMBER = pinNumber;
		diskList = new LinkedList<Disk>();
	}

	public void run() {
		try {
			running = true;
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode "+PIN_NUMBER+" in");
			while(running) {
				// TODO: need to find a way to receive input that disk is passin
				// TODO: need to find a way to receive color of said color
				boolean condition = false;
				if (condition) {
					int color = 0;
					diskList.add(new Disk(color));
				}
			}
		}
		catch (InterruptedException e) {
			System.out.println("The sensor thread got interrupted");
            Thread.currentThread().interrupt();
		}
		catch (Exception e) {

		}
	}

	public void stop() {
		running = false;
	}
}
