package Hardware;

import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

import javax.management.Query;

/** 
 * The class that will oversee the sensors and their input.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Sensor implements Runnable {
	final int PIN_NUMBER;
	private volatile boolean running = true;
	private volatile LinkedList<Disk> diskList;

	public Sensor(int pinNumber) {
		this.PIN_NUMBER = pinNumber;
		diskList = new LinkedList<Disk>();
	}

	public void run() {
		try {
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode "+PIN_NUMBER+" in");
			while(running) {
				if (/*disk is white*/) {
					diskList.add(new Disk(0));
				} else {
					diskList.add(new Disk(1));
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
