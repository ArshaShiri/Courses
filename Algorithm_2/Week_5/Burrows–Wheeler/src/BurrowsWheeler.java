import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

public class BurrowsWheeler {

    // apply Burrows-Wheeler transform,
    // reading from standard input and writing to standard output
    public static void transform() {

        StringBuilder sb = new StringBuilder();

        while (!BinaryStdIn.isEmpty()) {
            char character = BinaryStdIn.readChar();
            sb.append(character);
        }

        CircularSuffixArray csa = new CircularSuffixArray(sb.toString());

        // Find the row of the original string in the sorted suffixes
        int rowOfOriginalInput = 0;
        for (int i = 0; i < csa.length(); i++) {
            if (csa.index(i) == 0) {
                rowOfOriginalInput = i;
                break;
            }
        }

        BinaryStdOut.write(rowOfOriginalInput);

        for (int i = 0; i < csa.length(); i++) {
            int index = csa.index(i);
            if (index != 0) BinaryStdOut.write(sb.charAt(index - 1));
            else BinaryStdOut.write(sb.charAt(csa.length() - 1));
        }

        BinaryStdOut.close();
    }

    // apply Burrows-Wheeler inverse transform,
    // reading from standard input and writing to standard output
    public static void inverseTransform() {
        int first = BinaryStdIn.readInt();
        String burrowsWheelerTransform = BinaryStdIn.readString();

        int R = 256;
        int N = burrowsWheelerTransform.length();
        int[] count = new int[R + 1], next = new int[N];

        for (int i = 0; i < N; i++)
            count[burrowsWheelerTransform.charAt(i) + 1]++;

        for (int i = 1; i < R + 1; i++)
            count[i] += count[i - 1];

        for (int i = 0; i < N; i++)
            next[count[burrowsWheelerTransform.charAt(i)]++] = i;

        for (int i = next[first], c = 0; c < N; i = next[i], c++)
            BinaryStdOut.write(burrowsWheelerTransform.charAt(i));

        BinaryStdOut.close();
    }

    // if args[0] is "-", apply Burrows-Wheeler transform
    // if args[0] is "+", apply Burrows-Wheeler inverse transform
    public static void main(String[] args) {
        String action = args[0];
        if (action.equals("-")) {
            transform();
        } else if (action.equals("+")) {
            inverseTransform();
        } else {
            throw new IllegalArgumentException("Illegal action!");
        }
    }
}
