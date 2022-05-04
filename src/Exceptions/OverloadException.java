package Exceptions;

public class OverloadException extends Exception {
	public OverloadException() {
		super("Overload Exception: There are too many discs passing through the belt. Try removing a few to resolve this problem.");
	}
}