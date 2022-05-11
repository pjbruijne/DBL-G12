package ErrorHandling.Errors;

/** 
 * The error handling the case in which there is a problem with the wiring or signals between the rpi and the robot. It is a subclass of {@link java.lang.Error Error}. 
 * @author Peter de Bruijne
 * @author Tjeerd Roks
 */
public class SignalError extends Error {
	public SignalError() {
		super("Signal Error: The RPI lost the signal to the robot or the motor. The program will therefore forcefully shut down. Try reconnecting the wires or replacing them.");
	}
	public SignalError(String errorMsg) {
		super(errorMsg);
	}
}
