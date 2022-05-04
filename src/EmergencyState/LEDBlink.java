package EmergencyState;

public class LEDBlink {

    private int pinNumber; // WiringPin diagram
    private int blinkSpeed; // in miliseconds

    public LEDBlink(int pinNumber, int blinkSpeed) {
        this.pinNumber = pinNumber;
        this.blinkSpeed = blinkSpeed;
    }

    public void Blink() {
        try {
           Runtime runTime = Runtime.getRuntime();
            runTime.exec("gpio mode " + pinNumber + " out");
           while(true) {
               runTime.exec("gpio write " + pinNumber + " 1");
               Thread.sleep(blinkSpeed);
               runTime.exec("gpio write " + pinNumber + " 0");
               Thread.sleep(blinkSpeed);
            }
        } catch (Exception e) {
            // TODO: Error handling!
            System.out.println("Exception occured: " + e.getMessage());
        }
    }
}