import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private WeightedQuickUnionUF fullUnionFind_;
    private WeightedQuickUnionUF gridUnionFind_;
    private boolean[] isOpen_;
    private int sizeOfRowsAndColumns_;
    private int gridSize_;
    private int numberOfOpenSitesExcludingVirtualSites_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private int getArrayIndexFromRowAndColumn_(int row, int col) {
        // A site located in row i and column j is equivalent to a component located in the position
        // (row - 1) * sizeOfRowsAndColumns + col
        validateGridArrayIndex_(row, col);
        return (row - 1) * sizeOfRowsAndColumns_ + col;
    }

    private void validateGridArrayIndex_(int row, int col) {
        // Check if the given row and column are valid.
        if (row < 1 || row > sizeOfRowsAndColumns_) {
            throw new IllegalArgumentException("Row index " + row + " is not between 1 and " + (sizeOfRowsAndColumns_));
        }
        if (col < 1 || col > sizeOfRowsAndColumns_) {
            throw new IllegalArgumentException("Col index " + col + " is not between 1 and " + (sizeOfRowsAndColumns_));
        }
    }

    private void tryUnion_(int arrayIndex1, int row, int col) {
        if (isOpen(row, col)) {
            int arrayIndex2 = getArrayIndexFromRowAndColumn_(row, col);
            if (!isOpen_[arrayIndex2]) {
                numberOfOpenSitesExcludingVirtualSites_++;
            }
            gridUnionFind_.union(arrayIndex1, arrayIndex2);
            fullUnionFind_.union(arrayIndex1, arrayIndex2);
        }
    }

    private void connectOpenNeighbours_(int row, int col, int arrayIndex) {
        boolean isSiteOnFirstRow = row == 1;
        boolean isSiteOnLastRow = row == sizeOfRowsAndColumns_;

        // Special case where the size of the grid is 1.
        if (sizeOfRowsAndColumns_ == 1) {
            fullUnionFind_.union(arrayIndex, 0);
            gridUnionFind_.union(arrayIndex, 0);
            gridUnionFind_.union(arrayIndex, gridSize_ + 1);
            return;
        }
        if (isSiteOnFirstRow) {
            isOpen_[0] = true;
            fullUnionFind_.union(arrayIndex, 0);
            gridUnionFind_.union(arrayIndex, 0);
            tryUnion_(arrayIndex, row + 1, col);
        }
        if (isSiteOnLastRow) {
            isOpen_[gridSize_ + 1] = true;
            gridUnionFind_.union(arrayIndex, gridSize_ + 1);
            tryUnion_(arrayIndex, row - 1, col);
        }

        boolean siteIsNotOnFirstOrLastRow = !(isSiteOnFirstRow || isSiteOnLastRow);
        if (siteIsNotOnFirstOrLastRow) {
            tryUnion_(arrayIndex, row + 1, col);
            tryUnion_(arrayIndex, row - 1, col);
        }

        if (col == 1) {
            tryUnion_(arrayIndex, row, col + 1);
        } else if (col == sizeOfRowsAndColumns_) {
            tryUnion_(arrayIndex, row, col - 1);
        } else {
            tryUnion_(arrayIndex, row, col + 1);
            tryUnion_(arrayIndex, row, col - 1);
        }
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // creates n-by-n grid, with all sites initially blocked
    public Percolation(int n) {
        if (n <= 0) {
            throw new IllegalArgumentException("Grid size " + n + " is not valid ");
        }
        gridSize_ = n * n;
        sizeOfRowsAndColumns_ = n;
        numberOfOpenSitesExcludingVirtualSites_ = 0;
        isOpen_ = new boolean[gridSize_ + 2];

        // All sites initially blocked.
        for (int i = 0; i < gridSize_; ++i) {
            isOpen_[i] = false;
        }

        // Take the two additional virtual sites into account.
        gridUnionFind_ = new WeightedQuickUnionUF(gridSize_ + 2);

        // Another union find object to avoid back wash problem.
        // Only the virtual node at the top is needed to check if a path is open.
        fullUnionFind_ = new WeightedQuickUnionUF(gridSize_ + 1);
    }

    // opens the site (row, col) if it is not open already
    public void open(int row, int col) {
        int arrayIndex = getArrayIndexFromRowAndColumn_(row, col);
        if (isOpen_[arrayIndex]) {
            return;
        }
        numberOfOpenSitesExcludingVirtualSites_++;
        isOpen_[arrayIndex] = true;
        connectOpenNeighbours_(row, col, arrayIndex);
    }

    // Is the site (row, col) open?
    public boolean isOpen(int row, int col) {
        int arrayIndex = getArrayIndexFromRowAndColumn_(row, col);
        return isOpen_[arrayIndex];
    }

    // is the site (row, col) full?
    // A full site is an open site that can be connected to an open site in the top row via a chain of neighboring
    // (left, right, up, down) open sites. We say the system percolates if there is a full site in the bottom row.
    public boolean isFull(int row, int col) {
        int index = getArrayIndexFromRowAndColumn_(row, col);
        return fullUnionFind_.connected(0, index);
    }

    // returns the number of open sites
    public int numberOfOpenSites() {
        return numberOfOpenSitesExcludingVirtualSites_;
    }

    // does the system percolate?
    public boolean percolates() {
        return gridUnionFind_.connected(0, gridSize_ + 1);
    }

    // test client (optional)
    public static void main(String[] args) {
        Percolation percolation = new Percolation(5);
        percolation.open(1, 1);
        percolation.open(1, 2);
        percolation.open(2, 2);
        percolation.open(2, 3);
        percolation.open(3, 3);
        percolation.open(3, 2);
        percolation.open(3, 1);
        percolation.open(4, 1);
        percolation.open(4, 2);
        percolation.open(5, 2);
        percolation.open(5, 4);
        StdOut.println(percolation.percolates());
        StdOut.println(percolation.numberOfOpenSites() == 11);
        // Must be false
        StdOut.println(percolation.isFull(5, 4));
        // Must be true
        StdOut.println(percolation.isFull(5, 2));
    }
}
