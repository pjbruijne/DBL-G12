package ErrorHandling.Errors;

public class SignalError extends Error {
	public SignalError() {
		super("Signal Error: The RPI lost the signal to the robot or the motor. The program will therefore forcefully shut down. Try reconnecting the wires or replacing them.");
	}
}
