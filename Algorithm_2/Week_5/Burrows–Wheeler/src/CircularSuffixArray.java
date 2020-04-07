import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {

    // *** *** *** *** *** Private Attributes *** *** *** *** *** //

    private circularSuffixSort_ circularSuffixSort_;

    // *** *** *** *** *** Private Classes *** *** *** *** *** //

    // Copyright 2002-2020, Robert Sedgewick and Kevin Wayne.
    // https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/Quick3way.java.html
    // Modified version of Quick3way for sorting circular suffix arrays.

    private class circularSuffixSort_ {

        // We illustrate the behaviour of class with an example:
        // Suppose the input string is "abcdefg" with length 7.
        // The circular suffixes are as follow:
        // abcdefg (0)
        // bcdefga (1)
        // cdefgab (2)
        // defgabc (3)
        // efgabcd (4)
        // fgabcde (5)
        // gabcdef (6)
        // We assume that the above suffixes are created from the input string and sort them.
        // We initialize an array of indices corresponding to each suffix. The array entries keep track
        // of each suffix.
        // In this example the sorted array would be of size 7 from 0 to 6.

        private String inputString_;
        private int[] suffixArray_;
        private int stringLength_;


        private circularSuffixSort_(String s) {
            stringLength_ = s.length();
            inputString_ = s;
            suffixArray_ = new int[stringLength_];

            for (int i = 0; i < suffixArray_.length; i++) {
                suffixArray_[i] = i;
            }
        }

        private int characterAt_(int suffixNumber, int offset) {
            if (offset > stringLength_) return -1;
            int indexNumber = (suffixNumber + offset) % stringLength_;
            if (indexNumber >= stringLength_) {
                throw new IllegalArgumentException("Out of bound character for the string!");
            }
            return inputString_.charAt(indexNumber);
        }

        private void sort_() {
            sort_(inputString_, 0, inputString_.length() - 1, 0);
        }

        private void sort_(String s, int lo, int hi, int offset) {
            if (hi <= lo) return;
            int lt = lo;
            int gt = hi;
            int v = characterAt_(suffixArray_[lo], offset);
            int i = lo + 1;

            while (i <= gt) {
                int t = characterAt_(suffixArray_[i], offset);
                if (t < v) exchSuffixes_(lt++, i++);
                else if (t > v) exchSuffixes_(i, gt--);
                else i++;
            }

            sort_(s, lo, lt - 1, offset);
            if (v >= 0) sort_(s, lt, gt, offset + 1);
            sort_(s, gt + 1, hi, offset);
        }

        private void exchSuffixes_(int i, int j) {
            int swap = suffixArray_[j];
            suffixArray_[j] = suffixArray_[i];
            suffixArray_[i] = swap;
        }
    }

    // circular suffix array of s
    public CircularSuffixArray(String s) {
        if (s == null)
            throw new IllegalArgumentException("Input string is null!");
        circularSuffixSort_ = new circularSuffixSort_(s);
        circularSuffixSort_.sort_();
    }

    // length of s
    public int length() {
        return circularSuffixSort_.stringLength_;
    }

    // returns index of ith sorted suffix
    public int index(int i) {
        if (i < 0 || i >= length())
            throw new IllegalArgumentException("Invalid index!");
        return circularSuffixSort_.suffixArray_[i];
    }

    // unit testing (required)
    public static void main(String[] args) {

        String s = "ABRACADABRA!";
        CircularSuffixArray c = new CircularSuffixArray(s);
        for (int i = 0; i < s.length(); i++) {
            StdOut.print(c.index(i) + " ");
        }
    }
}
