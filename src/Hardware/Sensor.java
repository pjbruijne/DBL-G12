package Hardware;

import java.util.ArrayList;

/** 
 * The class that will oversee the sensors and their input.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class Sensor implements Runnable {
	final int PIN_NUMBER;
	private volatile boolean running = true;

	ArrayList<Disk> stack;

	public Sensor(int pinNumber) {
		this.PIN_NUMBER = pinNumber;
		stack = new ArrayList<Disk>();
	}

	/**
     * The method to add a disk to the stack
     */
    public void addDisk(Disk disk) {
        stack.add(disk);
    }

	/**
	 * The method to remove the first disk from the stack
	 */
	public void removeDisk() {
		stack.remove(0);
	}

	public void run() {
		try {
			Runtime runtime = Runtime.getRuntime();
			runtime.exec("gpio mode "+PIN_NUMBER+" in");
			while(running) {

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
