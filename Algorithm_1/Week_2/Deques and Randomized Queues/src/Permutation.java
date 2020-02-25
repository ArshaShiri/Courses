import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class Permutation {
    public static void main(String[] args) {
        if (args.length != 1) {
            throw new IllegalArgumentException("Only one argument is required!");
        }

        int k = Integer.parseInt(args[0]);
        RandomizedQueue<String> stringRandomizedQueue = new RandomizedQueue<>();

        int numberOfStrings = 0;
        while (!StdIn.isEmpty()) {
            stringRandomizedQueue.enqueue(StdIn.readString());
            numberOfStrings++;
        }

        if (k > numberOfStrings) {
            throw new IllegalArgumentException("k should be smaller than n or equal to it!");
        }

        for (int i = 0; i < k; i++) {
            StdOut.print(stringRandomizedQueue.dequeue() + "\n");
        }
    }
}
