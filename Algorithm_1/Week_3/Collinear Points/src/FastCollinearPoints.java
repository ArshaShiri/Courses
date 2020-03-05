import edu.princeton.cs.algs4.In;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;

public class FastCollinearPoints {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private ArrayList<LineSegment> lineSegments_ = new ArrayList<LineSegment>();

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private void checkForDuplicatePoints_(Point[] points) {
        for (int index = 0; index < points.length - 1; ++index) {
            if (points[index].compareTo(points[index + 1]) == 0) {
                throw new IllegalArgumentException("Array has duplicate points!");
            }
        }
    }

    private boolean isSLopeAlreadyFound_(double slope, ArrayList<Double> slopes) {
        Iterator<Double> iter = slopes.iterator();
        while (iter.hasNext()) {
            if (slope == iter.next()) return true;
        }
        return false;
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // finds all line segments containing 4 or more points
    public FastCollinearPoints(Point[] points) {
        if (points == null) {
            throw new IllegalArgumentException("Points container cannot be null!");
        }
        int counter = 1;
        for (Point point : points) {
            if (point == null) throw new IllegalArgumentException("A null point cannot be accepted!");
        }

        // This is to avoid changing the passed data to the constructor.
        Point[] pointsCopy = points.clone();

        // Sort based on points.
        Arrays.sort(pointsCopy);
        checkForDuplicatePoints_(pointsCopy);
        int size = pointsCopy.length;

        // ArrayList<Point> visitedOrigins = new ArrayList<>();
        for (int index = 0; index < size; ++index) {
            Point origin = pointsCopy[index];

            // We make a copy of sorted pointsCopy.
            Point[] pointsToExamine = pointsCopy.clone();

            // We sort this new array based on the sloped of other points to the origin to be able to utilize the
            // algorithm. It is important to reinitialize pointsToExamine to the sorted pointCopy in each iteration
            // of this loop. This is because we want to keep the initial sorted order preserved.
            Arrays.sort(pointsToExamine, origin.slopeOrder());
            for (int pointsToExamineIndex = 1; pointsToExamineIndex < pointsToExamine.length - 1; ++pointsToExamineIndex) {
                Point currentPoint = pointsToExamine[pointsToExamineIndex];
                double currentSlope = origin.slopeTo(currentPoint);

                // Since the initial array is sorted base on the points, if at this stage we visit a point which is
                // smaller than the current origin it means that it was visited before. As a result, we should advance
                // in the array till we have a new slope. This is because we want to avoid adding the sub-segments of
                // a segment to the result.
                if (currentPoint.compareTo(origin) < 0) {
                    while (pointsToExamineIndex < pointsToExamine.length) {
                        currentPoint = pointsToExamine[pointsToExamineIndex];
                        double newSlope = origin.slopeTo(currentPoint);
                        if (newSlope != currentSlope) {
                            currentSlope = newSlope;
                            // Even if the slope changes for this point, the point might be visited before.
                            if (currentPoint.compareTo(origin) > 0)
                                break;
                        }
                        pointsToExamineIndex++;
                    }
                }

                // Origin + the initial starting point.
                int numberOfCollinearPoints = 2;

                // Try to find cluster of points with the same slope to the origin.
                for (int pointsToExamineIndexIndex2 = pointsToExamineIndex + 1;
                     pointsToExamineIndexIndex2 < pointsToExamine.length;
                     ++pointsToExamineIndexIndex2) {
                    double otherSlope = origin.slopeTo(pointsToExamine[pointsToExamineIndexIndex2]);
                    if (currentSlope == otherSlope) {
                        numberOfCollinearPoints++;
                        boolean endOfSubArrayHasReached = pointsToExamineIndexIndex2 == pointsToExamine.length - 1;
                        if (endOfSubArrayHasReached && numberOfCollinearPoints > 3) {
                            LineSegment lineSegment = new LineSegment(origin, pointsToExamine[pointsToExamineIndexIndex2]);
                            lineSegments_.add(lineSegment);
                            pointsToExamineIndex = pointsToExamineIndexIndex2;
                        }
                    } else {
                        if (numberOfCollinearPoints > 3) {
                            LineSegment lineSegment = new LineSegment(origin, pointsToExamine[pointsToExamineIndexIndex2 - 1]);
                            lineSegments_.add(lineSegment);
                            pointsToExamineIndex = pointsToExamineIndexIndex2 - 1;
                        }
                        break;
                    }
                }
            }
        }
    }

    // the number of line segments
    public int numberOfSegments() {
        return lineSegments_.size();
    }

    // the line segments
    public LineSegment[] segments() {
        return lineSegments_.toArray(new LineSegment[this.numberOfSegments()]);
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        int n = in.readInt();
        Point[] points = new Point[n];
        for (int i = 0; i < n; i++) {
            int x = in.readInt();
            int y = in.readInt();
            points[i] = new Point(x, y);
        }

        FastCollinearPoints fastCollinearPoints = new FastCollinearPoints(points);
        System.out.println(fastCollinearPoints.numberOfSegments());
        LineSegment[] segments = fastCollinearPoints.segments();

        for (LineSegment segment : segments) {
            System.out.println(segment.toString());
        }
    }
}
