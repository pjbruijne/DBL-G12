package ExceptionHandling;

class OverloadException extends Exception {
	public OverloadException() {
		super("There are too many discs passing through the belt. Try removing a few to resolve this problem.");
	}
}