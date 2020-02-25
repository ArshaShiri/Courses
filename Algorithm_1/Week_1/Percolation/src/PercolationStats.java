import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private double[] results_;
    private double mean_;
    private double var_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private int getNumberOfOpenSitesForPercolation_(int n) {
        int numberOfOpenSiteForPercolation = 0;
        Percolation percolation = new Percolation(n);

        while (!percolation.percolates()) {
            int row = StdRandom.uniform(n) + 1;
            int col = StdRandom.uniform(n) + 1;
            if (percolation.isOpen(row, col)) {
                continue;
            }
            percolation.open(row, col);
            numberOfOpenSiteForPercolation++;
        }
        return numberOfOpenSiteForPercolation;
    }

    // *** *** *** *** *** public methods *** *** *** *** *** //

    // perform independent trials on an n-by-n grid
    public PercolationStats(int n, int trials) {
        if (n <= 0 || trials <= 0) {
            throw new IllegalArgumentException("Invalid trials or n " + n + ",  " + trials);
        }

        int gridSize = n * n;
        results_ = new double[trials];
        for (int i = 0; i < trials; ++i) {
            results_[i] = (double) getNumberOfOpenSitesForPercolation_(n) / gridSize;
        }
        mean_ = StdStats.mean(results_);
        var_ = StdStats.stddev(results_);
    }

    // sample mean of percolation threshold
    public double mean() {
        return mean_;
    }

    // sample standard deviation of percolation threshold
    public double stddev() {
        return var_;
    }

    // low endpoint of 95% confidence interval
    public double confidenceLo() {
        return mean_ - (1.96 * var_ / Math.sqrt(results_.length));
    }

    // high endpoint of 95% confidence interval
    public double confidenceHi() {
        return mean_ + (1.96 * var_ / Math.sqrt(results_.length));
    }

    // test client (see below)
    public static void main(String[] args) {
    }

}
