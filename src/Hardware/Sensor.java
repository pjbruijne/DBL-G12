package Hardware;

import java.util.LinkedList;

/** 
 * The class that will oversee the sensors and their input.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Sensor implements Runnable {
	final int PIN_NUMBER_INFRARED;
	final int PIN_NUMBER_COLOR;
	private volatile boolean running;
	volatile LinkedList<Disk> diskList;

	public Sensor(int pinNumberInfrared, int pinNumberColor) {
		this.PIN_NUMBER_INFRARED = pinNumberInfrared;
		this.PIN_NUMBER_COLOR = pinNumberColor;
		diskList = new LinkedList<Disk>();
	}

	public void run() {
		try {
			running = true;
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode " + PIN_NUMBER_INFRARED + " in");
			runtime.exec("gpio mode " + PIN_NUMBER_COLOR + " in");
			while(running) {
				// TODO: need to find a way to receive input that disk is passin
				// TODO: need to find a way to receive color of said disk
				if(/** infrared senses a disk */) {
					// Turn on color sensor
					if(/** color is white */ ) {
						diskList.add(new Disk(0));
					} else {
						diskList.add(new Disk(1));
					}
					// Turn off color sensor
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
