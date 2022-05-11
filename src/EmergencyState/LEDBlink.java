package EmergencyState;

/**
 * The class to oversee the emergency light. It will be used in a seperate thread to allow for multiple routines happening simultaneously.
 * @author Tjeerd Roks
 * @author Peter de Bruijne
 */
public class LEDBlink implements Runnable {

    final private int PIN_NUMBER; // WiringPin diagram
    final private int BLINK_SPEED; // in miliseconds
    private volatile boolean running; // to turn blinking on and off

    // Constructor
    public LEDBlink(int pinNumber, int blinkSpeed) {
        this.PIN_NUMBER = pinNumber;
        this.BLINK_SPEED = blinkSpeed;
    }

    public void run() {
        try {
            running = true;
            Runtime runTime = Runtime.getRuntime();
            // Set the pin to receiving
            runTime.exec("gpio mode " + PIN_NUMBER + " out");
            while(running) {
               // Set the pin to high and wait
               runTime.exec("gpio write " + PIN_NUMBER + " 1");
               Thread.sleep(BLINK_SPEED);
               // Set the pin to low and wait
               runTime.exec("gpio write " + PIN_NUMBER + " 0");
               Thread.sleep(BLINK_SPEED);
            }
        } catch (InterruptedException e) {
            System.out.println("The LEDBlink thread got interrupted");
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