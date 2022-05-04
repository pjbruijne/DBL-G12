package ErrorHandling.Errors;

public class BlockadeError extends Error {
	public BlockadeError() {
		super("Blockade Error: The belt has been obstructed, thus no disc can pass through. For the safety of the system, the motor will turn off. Remove anything blocking the discs.");
	}
}
