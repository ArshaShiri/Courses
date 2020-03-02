import edu.princeton.cs.algs4.Bag;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.DirectedCycle;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import java.util.HashMap;
import java.util.Set;

public class WordNet {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private final HashMap<String, Bag<Integer>> nouneToId_;
    private final HashMap<Integer, String> idToSynset_;
    private SAP sap_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private int buildNoneToIdsMapAndGetNumberOfIds_(String synsets) {
        In in = new In(synsets);
        int numberOfComponentsInEachline = 3;

        int numberOfIds = 0;
        while (in.hasNextLine()) {
            numberOfIds++;
            String line = in.readLine();
            String[] idSynsetsAndDef = line.split(",", numberOfComponentsInEachline);
            int id = Integer.parseInt(idSynsetsAndDef[0]);
            String synset = idSynsetsAndDef[1];
            idToSynset_.put(id, synset);
            String[] nouns = synset.split(" ", 0);
            for (String noun : nouns) {
                Bag<Integer> foundIds = nouneToId_.get(noun);
                if (foundIds == null) {
                    Bag<Integer> ids = new Bag<>();
                    ids.add(id);
                    nouneToId_.put(noun, ids);
                } else {
                    foundIds.add(id);
                }
            }
        }

        return numberOfIds;
    }

    private void buildSAP_(String hypernyms, int numberOfIds) {
        Digraph digraph = new Digraph(numberOfIds);
        In in = new In(hypernyms);

        while (in.hasNextLine()) {
            String line = in.readLine();
            String[] idAndHypernyms = line.split(",", 0);
            int noundId = Integer.parseInt(idAndHypernyms[0]);

            for (int hypernymNumber = 1; hypernymNumber < idAndHypernyms.length; hypernymNumber++) {
                int hypernym = Integer.parseInt(idAndHypernyms[hypernymNumber]);
                digraph.addEdge(noundId, hypernym);
            }
        }

        // Check if there is cycle in the graph.
        DirectedCycle dc = new DirectedCycle(digraph);
        if (dc.hasCycle())
            throw new IllegalArgumentException("WordNet has a cycle!");

        // Check if we only have one root in the wordnet
        int numberOfRoots = 0;
        for (int vertex = 0; vertex < digraph.V(); ++vertex) {
            if (digraph.outdegree(vertex) == 0) {
                numberOfRoots++;
                if (numberOfRoots > 1) {
                    throw new IllegalArgumentException("WordNet has more than one root!");
                }
            }
        }

        sap_ = new SAP(digraph);
    }


    // constructor takes the name of the two input files
    public WordNet(String synsets, String hypernyms) {
        if (synsets == null || hypernyms == null)
            throw new IllegalArgumentException("synsets or hypernyms cannot be null");

        nouneToId_ = new HashMap<>();
        idToSynset_ = new HashMap<>();
        int numberOfIds = buildNoneToIdsMapAndGetNumberOfIds_(synsets);
        buildSAP_(hypernyms, numberOfIds);
    }

    // returns all WordNet nouns
    public Iterable<String> nouns() {
        return nouneToId_.keySet();
    }

    // is the word a WordNet noun?
    public boolean isNoun(String word) {
        if (word == null)
            throw new IllegalArgumentException("A null word cannot be queried in the wordnet");
        Set<String> nouns = nouneToId_.keySet();
        return nouns.contains(word);
    }

    // distance between nounA and nounB (defined below)
    public int distance(String nounA, String nounB) {
        if (nounA == null || nounB == null)
            throw new IllegalArgumentException("At least one noun in the distance method is null");
        Iterable<Integer> nounAIds = nouneToId_.get(nounA);
        Iterable<Integer> nounBIds = nouneToId_.get(nounB);
        return sap_.length(nounAIds, nounBIds);
    }

    // a synset (second field of synsets.txt) that is the common ancestor of nounA and nounB
    // in a shortest ancestral path (defined below)
    public String sap(String nounA, String nounB) {
        if (nounA == null || nounB == null)
            throw new IllegalArgumentException("At least one noun in the sap method is null");
        Iterable<Integer> nounAIds = nouneToId_.get(nounA);
        Iterable<Integer> nounBIds = nouneToId_.get(nounB);
        int ancestorId = sap_.ancestor(nounAIds, nounBIds);
        return idToSynset_.get(ancestorId);
    }

    // do unit testing of this class
    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        String nounA = "thing";
        String nounB = "gelatine";
        int dist = wordnet.distance(nounA, nounB);
        StdOut.printf("Distance is: %d \n", dist);
    }
}
