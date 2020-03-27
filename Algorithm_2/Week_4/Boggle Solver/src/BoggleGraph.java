import edu.princeton.cs.algs4.Graph;

import java.util.HashSet;

public class BoggleGraph {
// *** *** *** *** *** Private Attributes *** *** *** *** *** //

    private Graph graph_;
    private BoggleBoard board_;
    private int numberOfRows_;
    private int numberOfCols_;
    private Dictionary dictionary_;

// *** *** *** *** *** Private Classes *** *** *** *** *** //

    private class Words_ {
        private boolean[] marked_;
        private HashSet<String> words_;

        private void findAllWords_(Graph graph, int vertex, Dictionary.DictionaryNode node) {
            // A dfs like algorithm to find all words.
            // The main difference is that, a node is marked temporarily for all the recursive calls which are run
            // after the current iteration of loop. For the previous calls, the node is marked back to false.
            int rowNumber = vertex / numberOfCols_;
            int colNumber = vertex - rowNumber * numberOfCols_;
            char currentCharacter = board_.getLetter(rowNumber, colNumber);

            Dictionary.DictionaryNode nextNodeInDictionary = node.next()[currentCharacter - 65];
            if (nextNodeInDictionary == null) return;
            if (currentCharacter == 'Q') {
                Dictionary.DictionaryNode nodeForU = nextNodeInDictionary.next()['U' - 65];
                if (nodeForU == null) return;
                nextNodeInDictionary = nodeForU;
            }

            if (nextNodeInDictionary.val() != null)
                words_.add(dictionary_.getWordFromNumber(nextNodeInDictionary.val()));

            marked_[vertex] = true;
            for (int neighbor : graph.adj(vertex)) {
                if (!marked_[neighbor]) findAllWords_(graph, neighbor, nextNodeInDictionary);
            }
            marked_[vertex] = false;
        }

        private Words_(Graph graph, int source) {
            marked_ = new boolean[graph.V()];
            words_ = new HashSet<>();
            findAllWords_(graph, source, dictionary_.getRoot_());
        }

        private HashSet<String> getAllTheWords() {
            return words_;
        }
    }

// *** *** *** *** *** Private Methods *** *** *** *** *** //

    private int getArrayIndexFromRowAndCol_(int row, int col) {
        return row * numberOfCols_ + col;
    }

    private void buildBoggleConnections_(BoggleBoard board) {
        for (int row = 0; row < numberOfRows_; row++) {
            boolean isOnBottomBorder = row == numberOfRows_ - 1;
            for (int col = 0; col < numberOfCols_; col++) {
                boolean isOnLeftBorder = col == 0;
                boolean isOnRightBorder = col == numberOfCols_ - 1;

                int currentVertex = getArrayIndexFromRowAndCol_(row, col);

                if (!isOnRightBorder) graph_.addEdge(currentVertex, getArrayIndexFromRowAndCol_(row, col + 1));
                if (!isOnBottomBorder) graph_.addEdge(currentVertex, getArrayIndexFromRowAndCol_(row + 1, col));

                // Diagonal connections.
                if (!isOnBottomBorder) {
                    if (!isOnLeftBorder) graph_.addEdge(currentVertex, getArrayIndexFromRowAndCol_(row + 1, col - 1));
                    if (!isOnRightBorder) graph_.addEdge(currentVertex, getArrayIndexFromRowAndCol_(row + 1, col + 1));
                }
            }
        }
    }

    // *** *** *** *** *** Public Methods *** *** *** *** *** //

    public BoggleGraph(BoggleBoard board, Dictionary dictionary) {
        numberOfRows_ = board.rows();
        numberOfCols_ = board.cols();
        graph_ = new Graph(numberOfRows_ * numberOfCols_);
        board_ = board;
        dictionary_ = dictionary;
        buildBoggleConnections_(board);
    }

    public HashSet<String> getAllValidWordsInTheBoards() {
        HashSet<String> result = new HashSet<>();
        for (int source = 0; source < graph_.V(); source++) {
            Words_ words = new Words_(graph_, source);
            result.addAll(words.getAllTheWords());
        }

        return result;
    }

    public static void main(String[] args) {
    }
}
