package ErrorHandling.Exceptions;

/** 
 * The exception handling the case in which a disc has been put in the wrong box. It is a subclass of {@link java.lang.Exception Exception}.
 * @author Peter de Bruijne
 * @author Tjeerd Roks
 */
public class SortingException extends Exception {
	public SortingException() {
		super("Sorting Exception: One of the discs has been wrongfully sorted. You will have to find the odd one and manually put it in the right box.");
	}
	public SortingException(String errorMsg) {
		super(errorMsg);
	}
}
