package EmergencyState;

public class LEDBlink implements Runnable {

    final private int pinNumber; // WiringPin diagram
    final private int blinkSpeed; // in miliseconds
    private volatile boolean running = true; // to turn blinking on and off

    // Constructor
    public LEDBlink(int pinNumber, int blinkSpeed) {
        this.pinNumber = pinNumber;
        this.blinkSpeed = blinkSpeed;
    }

    public void run() {
        try {
           Runtime runTime = Runtime.getRuntime();
           // Set the pin to receiving
            runTime.exec("gpio mode " + pinNumber + " out");
           while(running) {
               // Set the pin to high and wait
               runTime.exec("gpio write " + pinNumber + " 1");
               Thread.sleep(blinkSpeed);
               // Set the pin to low and wait
               runTime.exec("gpio write " + pinNumber + " 0");
               Thread.sleep(blinkSpeed);
            }
        } catch (InterruptedException e) {
            System.out.println("Exception occured: " + e.getMessage());
            Thread.currentThread().interrupt();
        } 
        catch (Exception e) {
            // TODO: Error handling!
            System.out.println("Exception occured: " + e.getMessage());
        }
    }

    public void stop() {
        running = false;
    }

}