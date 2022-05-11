package ErrorHandling.Errors;

/** 
 * The error handling the case in which something is blocking the objects on the belt. It is a subclass of {@link java.lang.Error Error}.
 * @author Peter de Bruijne
 * @author Tjeerd Roks
 */
public class BlockadeError extends Error {
	public BlockadeError() {
		super("Blockade Error: The belt has been obstructed, thus no disc can pass through. For the safety of the system, the motor will turn off. Remove anything blocking the discs.");
	}
	public BlockadeError(String errorMsg) {
		super(errorMsg);
	}
}
