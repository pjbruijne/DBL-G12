import EmergencyState.*;
import ErrorHandling.*;

public class Main {
       
    public static void main(String[] args) {
        // Just a blinking led effect
        System.out.println("Press CTRL-C to exit");
        LEDBlink ledBlink = new LEDBlink(4, 500);
        ledBlink.Blink();
    }

}
