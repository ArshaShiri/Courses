import edu.princeton.cs.algs4.Picture;
import edu.princeton.cs.algs4.Stack;

public class SeamCarver {
    // *** *** *** *** *** Private Attributes *** *** *** *** *** //

    private Picture picture_;
    private double[][] energyMatrix_;
    private boolean isTransposed_ = false;
    private final static double energyOfBorderPixels_ = 1000.0;

    // *** *** *** *** *** Private Methods *** *** *** *** *** //

    private void validateVerticalSeam_(int[] seam) {
        // The size of the vertical seam should be equal to the picture's height.
        if (seam.length != height())
            throw new IllegalArgumentException("The distance of the seam is not valid!");

        int previousIndex = seam[0];
        for (int index : seam) {
            // The current pixel and the pixel in the next row should not have a distance more than 1.
            boolean distanceOfPixels = Math.abs(index - previousIndex) > 1;
            if (distanceOfPixels) {
                throw new IllegalArgumentException("Distance of the indices do is more than 1 in the seam!");
            }
            previousIndex = index;

            boolean indexIsWithinBounds = (0 <= index) && (index < width());
            if (!indexIsWithinBounds) {
                throw new IllegalArgumentException("Vertical seam index is out of bound!");
            }
        }
    }

    private Picture copyPicture_(Picture picture) {
        Picture newPicture = new Picture(picture.width(), picture.height());
        for (int y = 0; y < picture.height(); y++) {
            for (int x = 0; x < picture.width(); x++) {
                newPicture.setRGB(x, y, picture.getRGB(x, y));
            }
        }
        return newPicture;
    }

    private double getGradient_(int pixel1, int pixel2) {
        // Pixel1 corresponds to pixel (x-1, y) or (x, y-1)
        // Pixel2 corresponds to pixel (x+1, y) or (x, y+1)
        int pixel1Red = (pixel1 >> 16) & 0xFF;
        int pixel1Green = (pixel1 >> 8) & 0xFF;
        int pixel1Blue = (pixel1) & 0xFF;

        int pixel2Red = (pixel2 >> 16) & 0xFF;
        int pixel2Green = (pixel2 >> 8) & 0xFF;
        int pixel2Blue = (pixel2) & 0xFF;

        int deltaRed = pixel2Red - pixel1Red;
        int deltaGreen = pixel2Green - pixel1Green;
        int deltaBlue = pixel2Blue - pixel1Blue;

        return Math.pow(deltaRed, 2.0) + Math.pow(deltaGreen, 2.0) + Math.pow(deltaBlue, 2.0);
    }

    private void transposeEnergyMatrix_() {
        double[][] energyMatrixTransposed = new double[width()][height()];
        for (int y = 0; y < height(); y++) {
            for (int x = 0; x < width(); x++) {
                energyMatrixTransposed[x][y] = energyMatrix_[y][x];
            }
        }
        energyMatrix_ = energyMatrixTransposed;
    }

    private void transposePicture_() {
        Picture newPicture = new Picture(height(), width());
        for (int y = 0; y < height(); y++) {
            for (int x = 0; x < width(); x++) {
                newPicture.setRGB(y, x, picture_.getRGB(x, y));
            }
        }
        picture_ = newPicture;
    }

    private void transposeEnergyMatrixAndPicture_() {
        transposeEnergyMatrix_();
        transposePicture_();
        isTransposed_ = !isTransposed_;
    }

    private int[] doFindVerticalSeam_() {
        // Helper method for findVerticalSeam and findHorizontalSeam.
        // It is more efficient to write separate code for find horizontal seam. The matrix transpose is expensive.
        EnergyToAndEdgeToMatrices_ energyAndEdgeMatrices = new EnergyToAndEdgeToMatrices_();
        double[][] energyToMatrix = energyAndEdgeMatrices.getEnergyMatrix();
        int[][] edgeToMatrix = energyAndEdgeMatrices.getEdgeMatrix();

        // Find the index of min energy on the bottom row.
        int indexOfMinEnergy = 0;
        double minEnergy = Double.MAX_VALUE;
        double[] bottomRow = energyToMatrix[height() - 1];
        for (int index = 0; index < bottomRow.length; index++) {
            if (bottomRow[index] < minEnergy) {
                minEnergy = bottomRow[index];
                indexOfMinEnergy = index;
            }
        }

        // Store the index of the lowest energy pixel in each row.
        Stack<Integer> minEnergyXCoordPerRowFromBelow = new Stack<>();
        int indexOfMinEnergyInCurrentRow = indexOfMinEnergy;
        for (int y = height() - 1; y >= 0; y--) {
            minEnergyXCoordPerRowFromBelow.push(indexOfMinEnergyInCurrentRow);
            int[] currentRow = edgeToMatrix[y];
            indexOfMinEnergyInCurrentRow = currentRow[indexOfMinEnergyInCurrentRow];
        }

        if (minEnergyXCoordPerRowFromBelow.size() != height()) {
            throw new RuntimeException("Mismatch of size of the stack and seam");
        }

        int[] seam = new int[minEnergyXCoordPerRowFromBelow.size()];
        for (int indexSeam = 0; indexSeam < seam.length; indexSeam++) {
            seam[indexSeam] = minEnergyXCoordPerRowFromBelow.pop();
        }

        return seam;
    }

    private void updateEnergyMatrix_(int[] seam) {
        // This helper function is called right after a seam is removed.
        // It is used for both vertical and horizontal seam removal.
        // Therefore, the caller should make sure that the energy matrix should be
        // transposed or not.

        // Default is for updating after a vertical seam removal. In case of a horizontal seam removal, the
        // energy matrix should be transposed.

        double[][] oldEnergyMatrix = energyMatrix_;
        double[][] newEnergyMatrix;
        newEnergyMatrix = new double[height()][width()];

        for (int y = 0; y < height(); y++) {
            int pixelRemovedInThisRow = seam[y];
            for (int x = 0; x < width(); x++) {
                boolean shouldEnergyBeRecalculated = (x == pixelRemovedInThisRow) ||
                        (x == pixelRemovedInThisRow - 1) ||
                        (x == pixelRemovedInThisRow + 1);
                if (shouldEnergyBeRecalculated) {
                    newEnergyMatrix[y][x] = energy(x, y);
                } else {
                    if (x > pixelRemovedInThisRow)
                        newEnergyMatrix[y][x] = oldEnergyMatrix[y][x + 1];
                    else
                        newEnergyMatrix[y][x] = oldEnergyMatrix[y][x];
                }
            }
        }

        energyMatrix_ = newEnergyMatrix;
    }

    // *** *** *** *** *** Private Classes *** *** *** *** *** //

    private class EnergyToAndEdgeToMatrices_ {
        // This helper class is used for the computation of the minimum cumulative energy to reach each pixel from the
        // pixel above. It is important to mention that each pixel is connected to 3 pixel below it.

        private double[][] energyTo_;
        private int[][] edgeTo_;

        private void initializeEnergyAndEdgeMatrices_() {
            energyTo_ = new double[height()][width()];
            edgeTo_ = new int[height()][width()];

            // Set initial distance to infinity
            for (int y = 0; y < height(); y++) {
                for (int x = 0; x < width(); x++) {
                    energyTo_[y][x] = Double.MAX_VALUE;
                }
            }

            // We can use the fact that each pixel in the seam is reached from the row above.
            // We just need to store the number of the the column in which the pixel in the above row lies.
            // We initialize the distTo array for each row.
            for (int y = 0; y < height(); y++) {
                for (int x = 0; x < width(); x++) {
                    edgeTo_[y][x] = x;
                }
            }
        }

        private void updateEnergyToAndEdgeToMatrices_(double cumulativeEnergyOfPixelAbove,
                                                      int pixelFromX,
                                                      int pixelToCheckX,
                                                      int pixelToCheckY,
                                                      double[][] energyTo,
                                                      int[][] edgeTo) {
            // Helper function for EnergyToAndEdgeToMatrices_.
            double energyOfPixelToCheck = energyMatrix_[pixelToCheckY][pixelToCheckX];
            double cumulativeEnergyOfPixelToCheck = energyTo[pixelToCheckY][pixelToCheckX];
            double energyToReachThePixelToCheckFromAbove = cumulativeEnergyOfPixelAbove + energyOfPixelToCheck;

            if (energyToReachThePixelToCheckFromAbove < cumulativeEnergyOfPixelToCheck) {
                energyTo[pixelToCheckY][pixelToCheckX] = energyToReachThePixelToCheckFromAbove;
                edgeTo[pixelToCheckY][pixelToCheckX] = pixelFromX;
            }
        }

        private void calculateEnergyAndEdgeMatrices_() {
            for (int y = 0; y < height() - 1; y++) {
                boolean pixelIsOnTopROw = y == 0;
                for (int x = 0; x < width(); x++) {
                    double cumulativeEnergyOfPixelAbove;
                    if (pixelIsOnTopROw) {
                        cumulativeEnergyOfPixelAbove = energyOfBorderPixels_;
                    } else {
                        cumulativeEnergyOfPixelAbove = energyTo_[y][x];
                    }

                    // update the pixel exactly below the current pixel.
                    updateEnergyToAndEdgeToMatrices_(
                            cumulativeEnergyOfPixelAbove, x, x, y + 1, energyTo_, edgeTo_);

                    boolean pixelIsOnLeftBorder = x == 0;
                    boolean pixelIsOnRightBorder = x == width() - 1;
                    if (!pixelIsOnLeftBorder) {
                        updateEnergyToAndEdgeToMatrices_(
                                cumulativeEnergyOfPixelAbove, x, x - 1, y + 1, energyTo_, edgeTo_);
                    }
                    if (!pixelIsOnRightBorder) {
                        updateEnergyToAndEdgeToMatrices_(
                                cumulativeEnergyOfPixelAbove, x, x + 1, y + 1, energyTo_, edgeTo_);
                    }
                }
            }
        }

        private EnergyToAndEdgeToMatrices_() {
            initializeEnergyAndEdgeMatrices_();
            calculateEnergyAndEdgeMatrices_();
        }

        private double[][] getEnergyMatrix() {
            return energyTo_;
        }

        private int[][] getEdgeMatrix() {
            return edgeTo_;
        }
    }

    // *** *** *** *** *** Public Methods *** *** *** *** *** //

    // create a seam carver object based on the given picture
    public SeamCarver(Picture picture) {
        if (picture == null) {
            throw new IllegalArgumentException("picture is null!");
        }

        picture_ = copyPicture_(picture);

        // Set the energy matrix for each pixel
        energyMatrix_ = new double[height()][width()];
        for (int y = 0; y < height(); y++) {
            for (int x = 0; x < width(); x++) {
                energyMatrix_[y][x] = energy(x, y);
            }
        }
    }

    // current picture
    public Picture picture() {
        return copyPicture_(picture_);
    }

    // width of current picture
    public int width() {
        return picture_.width();
    }

    // height of current picture
    public int height() {
        return picture_.height();
    }

    // energy of pixel at column x and row y
    public double energy(int x, int y) {
        boolean xIsWithinBounds = (x >= 0) && (x < width());
        boolean yIsWithinBounds = (y >= 0) && (y < height());
        if (!xIsWithinBounds || !yIsWithinBounds) {
            throw new IllegalArgumentException("x or y is not within valid bounds!");
        }

        boolean xIsOnBorder = (x == 0) || (x == width() - 1);
        boolean yIsOnBorder = (y == 0) || (y == height() - 1);
        if (xIsOnBorder || yIsOnBorder) {
            return energyOfBorderPixels_;
        }

        int pixel1X = picture_.getRGB(x - 1, y);
        int pixel2X = picture_.getRGB(x + 1, y);
        double xGrad = getGradient_(pixel1X, pixel2X);

        int pixel1Y = picture_.getRGB(x, y - 1);
        int pixel2Y = picture_.getRGB(x, y + 1);
        double yGrad = getGradient_(pixel1Y, pixel2Y);

        return Math.sqrt(xGrad + yGrad);
    }

    // sequence of indices for horizontal seam
    public int[] findHorizontalSeam() {
        transposeEnergyMatrixAndPicture_();
        int[] seam = doFindVerticalSeam_();
        transposeEnergyMatrixAndPicture_();
        return seam;
    }

    // sequence of indices for vertical seam
    public int[] findVerticalSeam() {
        return doFindVerticalSeam_();
    }

    // remove horizontal seam from current picture
    public void removeHorizontalSeam(int[] seam) {
        if (seam == null) {
            throw new IllegalArgumentException("seam is null!");
        }
        transposeEnergyMatrixAndPicture_();
        removeVerticalSeam(seam);
        transposeEnergyMatrixAndPicture_();
    }

    // remove vertical seam from current picture
    public void removeVerticalSeam(int[] seam) {
        if (seam == null) {
            throw new IllegalArgumentException("seam is null!");
        }

        validateVerticalSeam_(seam);
        Picture newPicture = new Picture(width() - 1, height());
        for (int y = 0; y < height(); y++) {
            int pixelToRemoveInThisRow = seam[y];
            for (int x = 0; x < width() - 1; x++) {
                if (x < pixelToRemoveInThisRow) {
                    newPicture.setRGB(x, y, picture_.getRGB(x, y));
                } else {
                    newPicture.setRGB(x, y, picture_.getRGB(x + 1, y));
                }
            }
        }

        picture_ = newPicture;
        updateEnergyMatrix_(seam);
    }

    //  unit testing (optional)
    public static void main(String[] args) {

    }
}
