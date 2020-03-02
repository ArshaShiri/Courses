import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //
    private final WordNet wordnet_;

    // constructor takes a WordNet object
    public Outcast(WordNet wordnet) {
        if (wordnet == null)
            throw new IllegalArgumentException("wordnet cannot be null!");
        wordnet_ = wordnet;
    }

    // given an array of WordNet nouns, return an outcast
    public String outcast(String[] nouns) {
        String outcast = "";
        int maxDistance = Integer.MIN_VALUE;

        for (String noun : nouns) {
            int distance = 0;
            for (String otherNoun : nouns) {
                distance += wordnet_.distance(noun, otherNoun);
            }

            if (distance > maxDistance) {
                maxDistance = distance;
                outcast = noun;
            }
        }

        return outcast;
    }

    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        Outcast outcast = new Outcast(wordnet);
        for (int t = 2; t < args.length; t++) {
            In in = new In(args[t]);
            String[] nouns = in.readAllStrings();
            StdOut.println(args[t] + ": " + outcast.outcast(nouns));
        }
    }
}
