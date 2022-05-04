package Errors;

public class ObstructionError extends Error {
	public ObstructionError() {
		super("Obstruction Error: The belt has been obstructed. For the safety of the system, the motor will now turn off. Try to remove any parts jamming the belt.");
	}
}
