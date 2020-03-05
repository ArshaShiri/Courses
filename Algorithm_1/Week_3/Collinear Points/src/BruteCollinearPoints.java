import java.util.ArrayList;
import java.util.Arrays;

public class BruteCollinearPoints {
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

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // finds all line segments containing 4 points
    public BruteCollinearPoints(Point[] points) {
        if (points == null) {
            throw new IllegalArgumentException("Points container cannot be null!");
        }
        for (Point point : points) {
            if (point == null) throw new IllegalArgumentException("A null point cannot be accepted!");
        }

        Point[] pointsCopy = points.clone();
        Arrays.sort(pointsCopy);
        checkForDuplicatePoints_(pointsCopy);

        for (int index1 = 0; index1 < pointsCopy.length - 3; ++index1) {
            Point point1 = pointsCopy[index1];

            for (int index2 = index1 + 1; index2 < pointsCopy.length - 2; ++index2) {
                Point point2 = pointsCopy[index2];
                double slope1 = point1.slopeTo(point2);

                for (int index3 = index2 + 1; index3 < pointsCopy.length - 1; ++index3) {
                    Point point3 = pointsCopy[index3];
                    double slope2 = point1.slopeTo(point3);
                    if (slope1 != slope2) {
                        continue;
                    }

                    for (int index4 = index3 + 1; index4 < pointsCopy.length; ++index4) {
                        Point point4 = pointsCopy[index4];
                        double slope3 = point1.slopeTo(point4);
                        if (slope2 != slope3) {
                            continue;
                        }

                        LineSegment lineSegment = new LineSegment(point1, point4);

                        lineSegments_.add(lineSegment);
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
//        Point point1 = new Point(0, 0);
//        Point point2 = new Point(1, 1);
//        Point point3 = new Point(1, 2);
//        Point point4 = new Point(0, 2);
//        Point point5 = new Point(5, 5);
//        Point point6 = new Point(4, 4);
//        Point point7 = new Point(3, 3);
//        Point point8 = new Point(2, 2);
//        Point point9 = new Point(10, 10);
//        Point point10 = new Point(0, 3);
//        Point point11 = new Point(0, 4);
//        Point point12 = new Point(0, 6);
//
//        Point[] points = {point1, point2, point3, point4, point5, point6,
//                point7, point8, point9, point10, point11, point12};
//
//        BruteCollinearPoints bruteCollinearPoints = new BruteCollinearPoints(points);
//        System.out.println(bruteCollinearPoints.numberOfSegments());
//        LineSegment[] segments = bruteCollinearPoints.segments();
//
//        for (LineSegment segment : segments) {
//            System.out.println(segment.toString());
//        }
    }
}
