import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;
import edu.princeton.cs.algs4.StdDraw;

import java.util.LinkedList;

public class KdTree {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private Node root_;
    private int size_ = 0;
    private static double XMAX = 1.0;
    private static double XMIN = 0.0;
    private static double YMAX = 1.0;
    private static double YMIN = 0.0;

    // *** *** *** *** *** Private classes and methods *** *** *** *** *** //

    private static class Node {
        // Helper class for the nodes in kd tree.
        private Point2D p_;
        // To check which coordinate is used to split the branch of tree.
        boolean isSplitVertically_;
        // the left/bottom subtree
        private Node left_;
        // the right/top subtree
        private Node right_;
        // The splitting line is either vertical or horizontal, as a result the x or y coordinate of the line
        // (depending whether it is vertical or horizontal) can be determined from the stored point at the node.
        // As a result, it is sufficient to store the min and max of the splitting line.
        double maxSplittingLine_;
        double minSplittingLine_;

        private Node(Point2D point, double maxSplittingLine, double minSplittingLine, int level) {
            p_ = point;
            maxSplittingLine_ = maxSplittingLine;
            minSplittingLine_ = minSplittingLine;
            isSplitVertically_ = level % 2 == 0;
        }
    }

    static private class Plane {
        // Helper class to model the plan in which the points are inserted.
        private double xMax = XMAX;
        private double xMin = XMIN;
        private double yMax = YMAX;
        private double yMin = YMIN;
    }

    private Node put_(Node node,
                      Point2D point,
                      Plane plane,
                      double maxSplittingLine,
                      double minSplittingLine,
                      int level) {
        // Helper method for the public method insert of class KdTree.

        // If the current node is null, create a new node at this location.
        if (node == null) {
            size_++;
            return new Node(point, maxSplittingLine, minSplittingLine, level);
        }

        // If the point at this node is equal to the inserted point return.
        if (node.p_.equals(point)) {
            return node;
        }

        if (node.isSplitVertically_) {
            int comparator = Double.compare(point.x(), node.p_.x());
            if (comparator < 0) {
                plane.xMax = node.p_.x();
                node.left_ = put_(node.left_, point, plane, plane.xMax, plane.xMin, level + 1);
            } else {
                plane.xMin = node.p_.x();
                node.right_ = put_(node.right_, point, plane, plane.xMax, plane.xMin, level + 1);
            }
        } else {
            int comparator = Double.compare(point.y(), node.p_.y());
            if (comparator < 0) {
                plane.yMax = node.p_.y();
                node.left_ = put_(node.left_, point, plane, plane.yMax, plane.yMin, level + 1);
            } else {
                plane.yMin = node.p_.y();
                node.right_ = put_(node.right_, point, plane, plane.yMax, plane.yMin, level + 1);
            }
        }

        return node;
    }

    private void put_(Point2D point) {
        // Helper method for the public method insert of class KdTree.
        Plane plane = new Plane();
        // Since the root node splits the plan vertically, start the recursion by providing y coordinates.
        // This method starts the recursion.
        root_ = put_(root_, point, plane, plane.yMax, plane.yMin, 0);
    }

    private Node getNextNode_(Node node, double coord1, double coord2) {
        // Compares the coordinates and select the right child.
        int compare = Double.compare(coord1, coord2);
        if (compare < 0) return node.left_;
        else return node.right_;
    }

    private boolean contains_(Node node, Point2D point) {
        // Helper method for the public method contains of class KdTree.
        if (node == null) return false;
        if (node.p_.equals(point)) return true;

        if (node.isSplitVertically_) {
            Node nextNode = getNextNode_(node, point.x(), node.p_.x());
            return contains_(nextNode, point);
        } else {
            Node nextNode = getNextNode_(node, point.y(), node.p_.y());
            return contains_(nextNode, point);
        }
    }

    private boolean contains_(Point2D point) {
        // Helper method for the public method contains of class KdTree.
        // This method starts the recursion.
        return contains_(root_, point);
    }

    private int getRectangleLineIntersection_(double queryRectMax,
                                              double queryRectMin,
                                              double lineConstantComponent) {
        // Since the line is vertical or horizontal and the query rectangle is aligned with x and y axis we do not need
        // all the coordinates of the line and the rectangle.
        if (queryRectMin > lineConstantComponent) return 1;
        if (queryRectMax < lineConstantComponent) return -1;
        return 0;
    }

    private void range_(RectHV queryRect, Node node, LinkedList<Point2D> pointsInRange) {
        // Helper method for the public method range of class KdTree.
        if (node == null) return;

        Point2D currentPoint = node.p_;
        if (queryRect.contains(currentPoint)) pointsInRange.add(currentPoint);

        int comparator;
        if (node.isSplitVertically_) {
            // For an intersection between a query rectangle and a vertical line, only the x coordinates of the line
            // and max and min corners of the rectangle are important.
            comparator = getRectangleLineIntersection_(queryRect.xmax(), queryRect.xmin(), currentPoint.x());
        } else {
            // For an intersection between a query rectangle and a vertical line, only the y coordinates of the line
            // and max and min corners of the rectangle are important.
            comparator = getRectangleLineIntersection_(queryRect.ymax(), queryRect.ymin(), currentPoint.y());
        }

        switch (comparator) {
            case 1: {
                // Check only the right child.
                range_(queryRect, node.right_, pointsInRange);
                break;
            }
            case -1: {
                // Check only the left child.
                range_(queryRect, node.left_, pointsInRange);
                break;
            }
            default: {
                // Check both children.
                range_(queryRect, node.right_, pointsInRange);
                range_(queryRect, node.left_, pointsInRange);
                break;
            }
        }
    }

    private Iterable<Point2D> range_(RectHV rect) {
        // Helper method for the public method range of class KdTree.
        // This method starts the recursion.
        LinkedList<Point2D> pointsInRange = new LinkedList<Point2D>();
        range_(rect, root_, pointsInRange);
        return pointsInRange;
    }

    private static class NearestPointAndSquareDistance {
        // Helper class to keep the nearest point and its distance which is used in the nearest method of class KdTree.
        public void setDistance(double distance) {
            distance_ = distance;
        }

        public void setPoint(Point2D point) {
            point_ = point;
        }

        public double getSquaredDistance() {
            return distance_;
        }

        public Point2D getPoint() {
            return point_;
        }

        private double distance_ = Double.POSITIVE_INFINITY;
        private Point2D point_ = new Point2D(0.0, 0.0);
    }

    private double getNearestSquaredDistanceOfPointToVerticalOrHorizontalLine_(Point2D queryPoint, Node node) {
        // This helper function can be much more simpler if vector and dot products are used. Moreover, due to some
        // bugs in the automatic corrector of this assignment, if a new point is created in this function the test will
        // fail due to additional prints. As a result, there is quite a lot of code duplication here which could be
        // avoided.
        if (node.isSplitVertically_) {
            // Check if the projection of the point on line is on the line itself or it lies outside the line
            double queryPointY = queryPoint.y();
            double queryPointX = queryPoint.x();
            double nodeX = node.p_.x();
            boolean isProjectionOnTheLine = (queryPointY >= node.minSplittingLine_) && (queryPointY <= node.maxSplittingLine_);
            if (isProjectionOnTheLine) {
                double distanceToProjectionOnSplittingLine = queryPointX - nodeX;
                return distanceToProjectionOnSplittingLine * distanceToProjectionOnSplittingLine;
            } else {
                // The minimum distance is between the query points and one of the end points of the line.
                double dx = queryPointX - nodeX;
                double dyStart = queryPointY - node.minSplittingLine_;
                double dyEnd = queryPointY - node.maxSplittingLine_;
                double squaredDistanceToStartPoint = dx * dx + dyStart * dyStart;
                double squaredDistanceToEndPoint = dx * dx + dyEnd * dyEnd;
                ;
                return Math.min(squaredDistanceToStartPoint, squaredDistanceToEndPoint);
            }
        } else {
            // Check if the projection of the point on line is on the line itself or it lies outside the line
            double queryPointY = queryPoint.y();
            double queryPointX = queryPoint.x();
            double nodeY = node.p_.y();
            boolean isProjectionOnTheLine = (queryPoint.x() >= node.minSplittingLine_) && (queryPoint.x() <= node.maxSplittingLine_);
            if (isProjectionOnTheLine) {
                double distanceToProjectionOnSplittingLine = queryPoint.y() - node.p_.y();
                return distanceToProjectionOnSplittingLine * distanceToProjectionOnSplittingLine;
            } else {
                // The minimum distance is between the query points and one of the end points of the line.
                double dy = queryPointY - nodeY;
                double dxStart = queryPointX - node.minSplittingLine_;
                double dxEnd = queryPointX - node.maxSplittingLine_;
                double squaredDistanceToStartPoint = dy * dy + dxStart * dxStart;
                double squaredDistanceToEndPoint = dy * dy + dxEnd * dxEnd;
                return Math.min(squaredDistanceToStartPoint, squaredDistanceToEndPoint);
            }
        }
    }

    private NearestPointAndSquareDistance nearest_(Node node,
                                                   Point2D queryPoint,
                                                   NearestPointAndSquareDistance nearestPointAndSquaredDistance) {
        // Helper method for the public method nearest of class KdTree.
        if (node == null) return nearestPointAndSquaredDistance;

        // Check the distance of query point to the current node.
        double currentSquaredDistance = node.p_.distanceSquaredTo(queryPoint);

        if (currentSquaredDistance < nearestPointAndSquaredDistance.getSquaredDistance()) {
            nearestPointAndSquaredDistance.setDistance(currentSquaredDistance);
            nearestPointAndSquaredDistance.setPoint(node.p_);
        }

        int comparator;
        if (node.isSplitVertically_) {
            comparator = Double.compare(queryPoint.x(), node.p_.x());
        } else {
            comparator = Double.compare(queryPoint.y(), node.p_.y());
        }

        double squaredDistanceToProjectionOnSplittingLine =
                getNearestSquaredDistanceOfPointToVerticalOrHorizontalLine_(queryPoint, node);

        if (comparator >= 0) {
            nearest_(node.right_, queryPoint, nearestPointAndSquaredDistance);

            // Check the other child if only the minimum distance that has been found so far is bigger than the
            // distance of the query point to the splitting line.
            if (nearestPointAndSquaredDistance.getSquaredDistance() > squaredDistanceToProjectionOnSplittingLine) {
                nearest_(node.left_, queryPoint, nearestPointAndSquaredDistance);
            }
        } else {
            nearest_(node.left_, queryPoint, nearestPointAndSquaredDistance);

            // Check the other child if only the minimum distance that has been found so far is bigger than the
            // distance of the query point to the splitting line.
            if (nearestPointAndSquaredDistance.getSquaredDistance() > squaredDistanceToProjectionOnSplittingLine) {
                nearest_(node.right_, queryPoint, nearestPointAndSquaredDistance);
            }
        }

        return nearestPointAndSquaredDistance;
    }

    private Point2D nearest_(Point2D queryPoint) {
        // Helper method for the public method nearest of class KdTree.
        NearestPointAndSquareDistance nearestPointAndSquareDistance = new NearestPointAndSquareDistance();
        nearestPointAndSquareDistance = nearest_(root_, queryPoint, nearestPointAndSquareDistance);
        return nearestPointAndSquareDistance.getPoint();
    }

    private void drawVerticalLine_(Node node) {
        StdDraw.setPenRadius(.01);
        StdDraw.setPenColor(StdDraw.RED);
        StdDraw.line(node.p_.x(), node.minSplittingLine_, node.p_.x(), node.maxSplittingLine_);
    }

    private void drawHorizontalLine_(Node node) {
        StdDraw.setPenRadius(.01);
        StdDraw.setPenColor(StdDraw.BLUE);
        StdDraw.line(node.minSplittingLine_, node.p_.y(), node.maxSplittingLine_, node.p_.y());
    }

    private void draw_(Node node) {
        if (node != null) {
            if (node.isSplitVertically_) {
                draw_(node.left_);
                drawVerticalLine_(node);
                draw_(node.right_);

                StdDraw.setPenColor(StdDraw.BLACK);
                StdDraw.setPenRadius(.025);
                node.p_.draw();
            } else {
                draw_(node.left_);
                drawHorizontalLine_(node);
                draw_(node.right_);

                StdDraw.setPenColor(StdDraw.BLACK);
                StdDraw.setPenRadius(.025);
                node.p_.draw();
            }
        }
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // construct an empty set of points
    public KdTree() {
    }

    // is the set empty?
    public boolean isEmpty() {
        return size_ == 0;
    }

    // number of points in the set
    public int size() {
        return size_;
    }

    // add the point to the set (if it is not already in the set)
    public void insert(Point2D p) {
        if (p == null) throw new IllegalArgumentException("Cannot insert a null point!");
        put_(p);
    }

    // does the set contain point p?
    public boolean contains(Point2D p) {
        if (p == null) throw new IllegalArgumentException("A null point cannot be queried!");
        return contains_(p);
    }

    // draw all points to standard draw
    public void draw() {
        StdDraw.clear();

        // The main square is 1.0 X 1.0
        draw_(root_);
    }

    // all points that are inside the rectangle (or on the boundary)
    public Iterable<Point2D> range(RectHV rect) {
        if (rect == null) throw new IllegalArgumentException("Range is null!");
        return range_(rect);
    }

    // a nearest neighbor in the set to point p; null if the set is empty
    public Point2D nearest(Point2D p) {
        if (p == null) throw new IllegalArgumentException("A null point cannot be queried!");
        if (size_ == 0) return null;
        return nearest_(p);
    }

    // unit testing of the methods (optional)
    public static void main(String[] args) {

        // initialize the data structures from file
        String filename = args[0];
        In in = new In(filename);
        // PointSET brute = new PointSET();
        KdTree kdtree = new KdTree();
        while (!in.isEmpty()) {
            double x = in.readDouble();
            double y = in.readDouble();
            Point2D p = new Point2D(x, y);
            kdtree.insert(p);
        }

        Point2D queryPoint = new Point2D(0.07, 0.13);
        Point2D nearestPoint = kdtree.nearest(queryPoint);

        double x0 = 0.0, y0 = 0.0;      // initial endpoint of rectangle
        double x1 = 0.0, y1 = 0.0;      // current location of mouse
        boolean isDragging = false;     // is the user dragging a rectangle

        // draw the points
        StdDraw.clear();
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(0.01);
        kdtree.draw();
        StdDraw.setPenColor(StdDraw.GREEN);
        StdDraw.setPenRadius(.02);
        queryPoint.draw();
        StdDraw.show();
    }
}
