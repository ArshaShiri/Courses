import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.SET;

import java.util.LinkedList;

public class PointSET {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //
    private SET<Point2D> pointSet_ = new SET<Point2D>();

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // construct an empty set of points
    public PointSET() {
    }

    // is the set empty?
    public boolean isEmpty() {
        return pointSet_.isEmpty();
    }

    // number of points in the set
    public int size() {
        return pointSet_.size();
    }

    // add the point to the set (if it is not already in the set)
    public void insert(Point2D p) {
        if (p == null) throw new IllegalArgumentException("Cannot insert a null point!");
        pointSet_.add(p);
    }

    // does the set contain point p?
    public boolean contains(Point2D p) {
        if (p == null) throw new IllegalArgumentException("A null point cannot be queried!");
        return pointSet_.contains(p);
    }

    // draw all points to standard draw
    public void draw() {
        for (Point2D point : pointSet_) {
            point.draw();
        }
    }

    // all points that are inside the rectangle (or on the boundary)
    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) throw new IllegalArgumentException("Range is null!");
        LinkedList<Point2D> pointsInRange = new LinkedList<Point2D>();
        if (size() == 0) return null;

        for (Point2D point2D : pointSet_) {
            if (rect.contains(point2D)) pointsInRange.add(point2D);
        }
        return pointsInRange;
    }

    // a nearest neighbor in the set to point p; null if the set is empty
    public Point2D nearest(Point2D p) {
        if (p == null) throw new IllegalArgumentException("A null point cannot be queried!");
        double distanceSquared = Double.POSITIVE_INFINITY;
        Point2D nearestPoint = null;

        for (Point2D point : pointSet_) {
            double currentDistance = p.distanceSquaredTo(point);
            if (currentDistance < distanceSquared) {
                nearestPoint = point;
                distanceSquared = currentDistance;
            }
        }

        return nearestPoint;
    }

    // unit testing of the methods (optional)
    public static void main(String[] args) {
        Point2D p1 = new Point2D(0.25, 0.5);
        Point2D p2 = new Point2D(0.25, 1.0);
        Point2D p3 = new Point2D(1.0, 0.0);
        Point2D p4 = new Point2D(0.25, 0.0);
        Point2D p5 = new Point2D(0.5, 0.25);
        Point2D p6 = new Point2D(0.5, 0.0);
        Point2D p7 = new Point2D(0.0, 0.25);
        Point2D p8 = new Point2D(1.0, 0.75);
        Point2D p9 = new Point2D(0.75, 0.5);
        Point2D p10 = new Point2D(0.0, 0.75);
        PointSET pointSet = new PointSET();
        pointSet.insert(p1);
        pointSet.insert(p2);
        pointSet.insert(p3);
        pointSet.insert(p4);
        pointSet.insert(p5);
        pointSet.insert(p6);
        pointSet.insert(p7);
        pointSet.insert(p8);
        pointSet.insert(p9);
        pointSet.insert(p10);

        Point2D nearest = pointSet.nearest(p5);
        Point2D t = new Point2D(0.0, 0.0);
    }
}
