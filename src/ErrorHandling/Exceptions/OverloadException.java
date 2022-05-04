package ErrorHandling.Exceptions;

/** 
 * The exception handling the case in which there are too many discs stacked together on the belt. It is a subclass of {@link java.lang.Exception Exception}.
 * @author Peter de Bruijne
 * @author Tjeerd Roks
 */
public class OverloadException extends Exception {
	public OverloadException() {
		super("Overload Exception: There are too many discs passing through the belt. Try removing a few to resolve this problem.");
	}
}