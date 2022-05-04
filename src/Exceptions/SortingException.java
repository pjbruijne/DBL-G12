package Exceptions;

public class SortingException extends Exception {
	public SortingException() {
		super("Sorting Exception: One of the discs has been wrongfully sorted. You will have to find the odd one and manually put it in the right box.");
	}
}
