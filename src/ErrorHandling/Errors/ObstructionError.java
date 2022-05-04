package ErrorHandling.Errors;

/** 
 * The error handling the case in which the belt is stuck. It is a subclass of {@link java.lang.Error Error}.
 * @author Peter de Bruijne
 * @author Tjeerd Roks
 */
public class ObstructionError extends Error {
	public ObstructionError() {
		super("Obstruction Error: The belt has been obstructed. For the safety of the system, the motor will now turn off. Try to remove any parts jamming the belt.");
	}
}
