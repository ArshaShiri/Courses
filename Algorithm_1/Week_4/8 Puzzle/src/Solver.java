import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.StdOut;

public class Solver {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private final Stack<Board> solutions_ = new Stack<Board>();
    private int numberOfMoves_;
    private boolean isSolvable_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private static class SearchNode_ implements Comparable<SearchNode_> {
        // This class represents a search node in A* algorithm.
        // *** *** *** *** *** Private attributes *** *** *** *** *** //
        private final Board board_;
        private SearchNode_ previousSearchNode_ = null;
        private int manhattan_;
        private int numberOfMoves_ = 0;

        SearchNode_(Board board) {
            board_ = board;
            manhattan_ = board.manhattan();
        }

        SearchNode_(Board board, SearchNode_ previousSearchNode) {
            board_ = board;
            previousSearchNode_ = previousSearchNode;
            numberOfMoves_ = previousSearchNode.numberOfMoves_ + 1;
            manhattan_ = board.manhattan();
        }

        public int compareTo(SearchNode_ searchNode) {
            // Defines a measurement to be used for priority queue.
            int manhattanPriorityFunctionOfThis = this.manhattan_ + this.numberOfMoves_;
            int manhattanPriorityFunctionOfThat = searchNode.manhattan_ + searchNode.numberOfMoves_;
            return manhattanPriorityFunctionOfThis - manhattanPriorityFunctionOfThat;
        }
    }

    private void updateQueue_(SearchNode_ currentSearchNode, MinPQ<SearchNode_> queue) {
        // Update the priority queue with the neighbors of the current board.
        Board board = currentSearchNode.board_;
        Iterable<Board> neighbors = currentSearchNode.board_.neighbors();

        for (Board neighbor : neighbors) {
            if (currentSearchNode.previousSearchNode_ != null) {
                // Check if the current neighbor is not the previous board. We do not want to add the previous board
                // again to the queue.
                if (neighbor.equals(currentSearchNode.previousSearchNode_.board_)) continue;
            }
            queue.insert(new SearchNode_(neighbor, currentSearchNode));
        }
    }

    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) {
        if (initial == null) throw new IllegalArgumentException("Initial board cannot be null!");

        MinPQ<SearchNode_> searchNodes = new MinPQ<SearchNode_>();
        MinPQ<SearchNode_> twinSearchNodes = new MinPQ<SearchNode_>();

        searchNodes.insert(new SearchNode_(initial));
        twinSearchNodes.insert(new SearchNode_(initial.twin()));

        while (true) {
            SearchNode_ currentSearchNode = searchNodes.delMin();
            SearchNode_ currentTwinSearchNode = twinSearchNodes.delMin();
            Board currentBoard = currentSearchNode.board_;
            Board twinBoard = currentTwinSearchNode.board_;

            // Check if we have reached the goal
            if (currentBoard.isGoal()) {
                numberOfMoves_ = currentSearchNode.numberOfMoves_;
                while (currentSearchNode != null) {
                    solutions_.push(currentSearchNode.board_);
                    currentSearchNode = currentSearchNode.previousSearchNode_;
                }

                isSolvable_ = true;
                break;
            } else if (twinBoard.isGoal()) {
                numberOfMoves_ = -1;
                isSolvable_ = false;
                break;
            }

            // update the queues
            updateQueue_(currentSearchNode, searchNodes);
            updateQueue_(currentTwinSearchNode, twinSearchNodes);
        }
    }

    // is the initial board solvable? (see below)
    public boolean isSolvable() {
        return isSolvable_;
    }

    // min number of moves to solve initial board
    public int moves() {
        return numberOfMoves_;
    }

    // sequence of boards in a shortest solution
    public Iterable<Board> solution() {
        if (!isSolvable_) return null;
        return solutions_;
    }

    // test client (see below)
    public static void main(String[] args) {
        // create initial board from file
        In in = new In(args[0]);
        int n = in.readInt();
        int[][] tiles = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                tiles[i][j] = in.readInt();
        Board initial = new Board(tiles);

        // solve the puzzle
        Solver solver = new Solver(initial);

        // print solution to standard output
        if (!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }
    }
}
