import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

public class MoveToFront {
    private static int numberOfCharacters_ = 256;

    // apply move-to-front encoding, reading from standard input and writing to standard output
    public static void encode() {
        char[] charPositions = new char[numberOfCharacters_];

        // Store the initial order of 256 extended ASCII characters
        for (int i = 0; i < numberOfCharacters_; i++) {
            charPositions[i] = (char) i;
        }

        while (!BinaryStdIn.isEmpty()) {
            char character = BinaryStdIn.readChar();

            // Find the character position.
            int characterPosition = 0;
            while (charPositions[characterPosition] != character) characterPosition++;
            BinaryStdOut.write((char) characterPosition);

            // Put the character at the beginning of the array.
            System.arraycopy(charPositions, 0, charPositions, 1, characterPosition);
            charPositions[0] = character;
        }

        BinaryStdOut.close();
    }

    // apply move-to-front decoding, reading from standard input and writing to standard output
    public static void decode() {
        int[] charPositions = new int[numberOfCharacters_];

        // Store the initial order of 256 extended ASCII characters
        for (int i = 0; i < numberOfCharacters_; i++) {
            charPositions[i] = (char) i;
        }

        while (!BinaryStdIn.isEmpty()) {
            int position = BinaryStdIn.readChar();
            char character = (char) charPositions[position];

            BinaryStdOut.write(character);

            // Put the character at the beginning of the array.
            System.arraycopy(charPositions, 0, charPositions, 1, position);
            charPositions[0] = character;
        }

        BinaryStdOut.close();
    }

    // if args[0] is "-", apply move-to-front encoding
    // if args[0] is "+", apply move-to-front decoding
    public static void main(String[] args) {
        String action = args[0];
        if (action.equals("-")) {
            encode();
        } else if (action.equals("+")) {
            decode();
        } else {
            throw new IllegalArgumentException("Illegal action!");
        }
    }
}
