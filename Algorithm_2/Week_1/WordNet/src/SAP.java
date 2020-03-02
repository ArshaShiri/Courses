import edu.princeton.cs.algs4.BreadthFirstDirectedPaths;
import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.HashMap;
import java.util.HashSet;

public class SAP {

    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private final Digraph G_;
    // Pair of vertices as a key and their common ancestor and length as value.
    private final HashMap<HashSet<Integer>, int[]> cache_;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private int[] getCommonAncestorAndLength_(BreadthFirstDirectedPaths BFS1, BreadthFirstDirectedPaths BFS2) {
        // Works for both single and multiple sources.
        int length = Integer.MAX_VALUE;
        int commonAncestor = -1;

        for (int currentVertex = 0; currentVertex < G_.V(); currentVertex++) {
            boolean isCurrentVertexReachableFromV = BFS1.hasPathTo(currentVertex);
            boolean isCurrentVertexReachableFromW = BFS2.hasPathTo(currentVertex);

            if (isCurrentVertexReachableFromV && isCurrentVertexReachableFromW) {
                int distanceFromV = BFS1.distTo(currentVertex);
                int distanceFromW = BFS2.distTo(currentVertex);
                int sumOfDistance = distanceFromV + distanceFromW;

                if (sumOfDistance < length) {
                    length = sumOfDistance;
                    commonAncestor = currentVertex;
                }
            }
        }

        int[] ancestorAndLength = new int[2];
        ancestorAndLength[0] = commonAncestor;

        if (length == Integer.MAX_VALUE) {
            // Add -1 for both the length if no common ancestor could be found.
            length = -1;
        }
        ancestorAndLength[1] = length;
        return ancestorAndLength;
    }

    private int[] getCommonAncestorAndLength_(int v, int w) {
        HashSet<Integer> cacheKey = new HashSet<>();
        cacheKey.add(v);
        cacheKey.add(w);
        int[] foundAncestorAndLength = cache_.get(cacheKey);

        if (foundAncestorAndLength != null) {
            return foundAncestorAndLength;
        }

        BreadthFirstDirectedPaths vBFS = new BreadthFirstDirectedPaths(G_, v);
        BreadthFirstDirectedPaths wBFS = new BreadthFirstDirectedPaths(G_, w);
        int[] ancestorAndLength = getCommonAncestorAndLength_(vBFS, wBFS);
        cache_.put(cacheKey, ancestorAndLength);

        return ancestorAndLength;
    }

    private int[] getCommonAncestorAndLength_(Iterable<Integer> v, Iterable<Integer> w) {
        BreadthFirstDirectedPaths vBFS = new BreadthFirstDirectedPaths(G_, v);
        BreadthFirstDirectedPaths wBFS = new BreadthFirstDirectedPaths(G_, w);
        return getCommonAncestorAndLength_(vBFS, wBFS);
    }

    private void validateVertexList_(Iterable<Integer> v) {
        for (Integer vertex : v) {
            if (vertex == null) {
                throw new IllegalArgumentException("Null point in the vertex list is not acceptable!");
            }
        }
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // constructor takes a digraph (not necessarily a DAG)
    public SAP(Digraph G) {
        if (G == null)
            throw new IllegalArgumentException("SAP cannot be constructed with null");
        G_ = new Digraph(G);
        cache_ = new HashMap<>();
    }

    // length of shortest ancestral path between v and w; -1 if no such path
    public int length(int v, int w) {
        return getCommonAncestorAndLength_(v, w)[1];
    }

    // a common ancestor of v and w that participates in a shortest ancestral path; -1 if no such path
    public int ancestor(int v, int w) {
        return getCommonAncestorAndLength_(v, w)[0];
    }

    // length of shortest ancestral path between any vertex in v and any vertex in w; -1 if no such path
    public int length(Iterable<Integer> v, Iterable<Integer> w) {
        if (v == null || w == null)
            throw new IllegalArgumentException("Point subset cannot be null!");
        validateVertexList_(v);
        validateVertexList_(w);
        return getCommonAncestorAndLength_(v, w)[1];
    }

    // a common ancestor that participates in shortest ancestral path; -1 if no such path
    public int ancestor(Iterable<Integer> v, Iterable<Integer> w) {
        if (v == null || w == null)
            throw new IllegalArgumentException("Point subset cannot be null!");
        validateVertexList_(v);
        validateVertexList_(w);
        return getCommonAncestorAndLength_(v, w)[0];
    }

    // do unit testing of this class
    public static void main(String[] args) {
        In in = new In(args[0]);
        Digraph G = new Digraph(in);
        SAP sap = new SAP(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length = sap.length(v, w);
            int ancestor = sap.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }
    }
}
