import java.util.Arrays;
import java.util.LinkedList;

public class Board {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private int rowOfZero_;
    private int colOfZero_;
    private final int n_;
    private final int[][] tiles_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private static int getAbsoluteValue_(int val) {
        if (val >= 0) return val;
        else return -val;
    }

    private static int[][] deepCopy_(int[][] original) {
        // Deep copy of 2D array.
        if (original == null) {
            return null;
        }

        final int[][] result = new int[original.length][];
        for (int i = 0; i < original.length; i++) {
            result[i] = Arrays.copyOf(original[i], original[i].length);
        }

        return result;
    }

    private static int[][] getNewBoardWithSwapElements(
            int[][] oldTIle, int row1, int col1, int row2, int col2) {
        // This method swaps the element located in row1 and col1 with element in
        // row2 and col2.

        int[][] newTiles = deepCopy_(oldTIle);
        int temp = newTiles[row1][col1];
        newTiles[row1][col1] = newTiles[row2][col2];
        newTiles[row2][col2] = temp;
        return newTiles;
    }

    private int getDistanceToFinalLocation_(int tile, int currentRow, int currentCol) {
        // This method gives the vertical distance plus horizontal distance of a tile to its final location
        // in a solve puzzle.
        // According to the final solution, the row and column of each entry can be calculated.
        // The final solution for a 3X3 puzzle looks like below:
        // 1 2 3
        // 4 5 6
        // 7 8 0

        if (tile == 0) return 0;
        int rowInFinalSolution = (tile - 1) / n_;
        int colInFinalSolution = (tile - 1) - rowInFinalSolution * n_;
        return getAbsoluteValue_(rowInFinalSolution - currentRow) +
                getAbsoluteValue_(colInFinalSolution - currentCol);
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // create a board from an n-by-n array of tiles,
    // where tiles[row][col] = tile at (row, col)
    public Board(int[][] tiles) {

        if (tiles == null) {
            throw new IllegalArgumentException("Tiles cannot be null!");
        }

        n_ = tiles[0].length;
        tiles_ = deepCopy_(tiles);

        // Get the row and the col for the element zero (empty space in the board)
        for (int col = 0; col < this.n_; col++) {
            for (int row = 0; row < n_; row++) {
                if (tiles_[row][col] == 0) {
                    rowOfZero_ = row;
                    colOfZero_ = col;
                    break;
                }
            }
        }
    }

    // string representation of this board
    public String toString() {
        StringBuilder board = new StringBuilder();
        board.append(n_).append("\n");

        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < n_; j++) {
                board.append(String.format("%2d ", tiles_[i][j]));
            }
            board.append("\n");
        }
        return board.toString();
    }

    // board dimension n
    public int dimension() {
        return n_;
    }

    // To measure how close a board is to the goal board, we define two notions of distance.

    // number of tiles out of place
    public int hamming() {
        // The Hamming distance betweeen a board and the goal board is the number of tiles in the wrong position.
        int numberOfTilesOutOfPlace = 0;
        for (int row = 0; row < n_; row++) {
            for (int col = 0; col < n_; col++) {
                if (tiles_[row][col] == 0) continue;
                if (tiles_[row][col] != row * n_ + col + 1) numberOfTilesOutOfPlace++;
            }
        }

        return numberOfTilesOutOfPlace;
    }

    // sum of Manhattan distances between tiles and goal
    public int manhattan() {
        //  The Manhattan distance between a board and the goal board is the sum of the Manhattan distances
        //  (sum of the vertical and horizontal distance) from the tiles to their goal positions.
        int manhattanCoefficient = 0;
        for (int row = 0; row < n_; row++) {
            for (int col = 0; col < n_; col++) {
                manhattanCoefficient += getDistanceToFinalLocation_(tiles_[row][col], row, col);
            }
        }

        return manhattanCoefficient;
    }

    // is this board the goal board?
    public boolean isGoal() {
        return hamming() == 0;
    }

    // does this board equal y?
    public boolean equals(Object y) {
        if (y == this) return true;
        if (y == null) return false;
        if (y.getClass() != this.getClass())
            return false;

        Board that = (Board) y;
        if (this.dimension() != that.dimension()) return false;

        int[][] thisTiles = this.tiles_;
        int[][] thatTiles = that.tiles_;
        for (int col = 0; col < this.n_; col++) {
            for (int row = 0; row < n_; row++) {
                if (thisTiles[row][col] != thatTiles[row][col]) return false;
            }
        }

        return true;
    }

    // all neighboring boards
    public Iterable<Board> neighbors() {
        // Gives the neighbors of the current board. Any possible swap between the zero element and another tile defines
        // a neighbor.

        LinkedList<Board> neighbors = new LinkedList<Board>();

        if (rowOfZero_ > 0)
            neighbors.add(new Board(getNewBoardWithSwapElements(tiles_, rowOfZero_, colOfZero_, rowOfZero_ - 1, colOfZero_)));
        if (rowOfZero_ < n_ - 1)
            neighbors.add(new Board(getNewBoardWithSwapElements(tiles_, rowOfZero_, colOfZero_, rowOfZero_ + 1, colOfZero_)));

        if (colOfZero_ > 0)
            neighbors.add(new Board(getNewBoardWithSwapElements(tiles_, rowOfZero_, colOfZero_, rowOfZero_, colOfZero_ - 1)));
        if (colOfZero_ < n_ - 1)
            neighbors.add(new Board(getNewBoardWithSwapElements(tiles_, rowOfZero_, colOfZero_, rowOfZero_, colOfZero_ + 1)));

        return neighbors;
    }

    // a board that is obtained by exchanging any pair of tiles (number zero is not a tile)
    public Board twin() {
        for (int row = 0; row < n_; row++) {
            for (int col = 0; col < n_ - 1; col++) {
                if (row != rowOfZero_)
                    return new Board(getNewBoardWithSwapElements(tiles_, row, col, row, col + 1));
            }
        }

        throw new RuntimeException("Cannot acquire the twin board");
    }

    public static void main(String[] args) {
    }
}
