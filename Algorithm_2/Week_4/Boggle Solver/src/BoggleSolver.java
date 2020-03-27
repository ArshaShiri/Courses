import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class BoggleSolver {
    // *** *** *** *** *** Private Attributes *** *** *** *** *** //

    private Dictionary dictionary_;
    private BoggleGraph graph_;

    // *** *** *** *** *** Public Methods *** *** *** *** *** //

    // Initializes the data structure using the given array of strings as the dictionary.
    // (You can assume each word in the dictionary contains only the uppercase letters A through Z.)
    public BoggleSolver(String[] dictionary) {
        dictionary_ = new Dictionary(dictionary);
    }

    // Returns the set of all valid words in the given Boggle board, as an Iterable.
    public Iterable<String> getAllValidWords(BoggleBoard board) {
        graph_ = new BoggleGraph(board, dictionary_);
        return graph_.getAllValidWordsInTheBoards();
    }

    // Returns the score of the given word if it is in the dictionary, zero otherwise.
    // (You can assume the word contains only the uppercase letters A through Z.)
    public int scoreOf(String word) {
        // word length 	points
        //     3–4        1
        //      5	      2
        //      6	      3
        //      7	      5
        //      8+	     11
        if (dictionary_.isWordInDictionary(word)) {
            int wordLength = word.length();
            switch (wordLength) {
                case 3:
                case 4:
                    return 1;
                case 5:
                    return 2;
                case 6:
                    return 3;
                case 7:
                    return 5;
                default:
                    return 11;
            }
        }
        return 0;
    }

    public static void main(String[] args) {
        // long startTime = System.nanoTime();

        In in = new In(args[0]);
        String[] dictionary = in.readAllStrings();
        BoggleSolver solver = new BoggleSolver(dictionary);
        BoggleBoard board = new BoggleBoard(args[1]);
        int score = 0;

        for (String word : solver.getAllValidWords(board)) {
            StdOut.println(word);
            score += solver.scoreOf(word);
        }

        StdOut.println("Score = " + score);
    }
}
