import java.util.Iterator;

public class Deque<Item> implements Iterable<Item> {
// *** *** *** *** *** Private attributes *** *** *** *** *** //

    private Node first_ = null;
    private Node last_ = null;
    private int size_ = 0;

    // *** *** *** *** *** Private classes *** *** *** *** *** //

    private class Node {
        Item item;
        Node next = null;
        Node previous = null;
    }

    private class DequeIterator implements Iterator<Item> {

        public boolean hasNext() {
            return current != null;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }

        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException("Next item does not exist in the iterator! ");
            }

            Item item = current.item;
            current = current.previous;
            return item;
        }

        private Node current = first_;
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // construct an empty deque
    public Deque() {
    }

    // is the deque empty?
    public boolean isEmpty() {
        return size_ == 0;
    }

    // return the number of items on the deque
    public int size() {
        return size_;
        //
    }

    // add the item to the front
    public void addFirst(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot add a null object to the beginning of the deque!");
        }

        Node oldFirst = first_;
        first_ = new Node();
        first_.item = item;

        if (size_ == 0) {
            last_ = first_;
        } else {
            first_.previous = oldFirst;
            oldFirst.next = first_;
        }
        size_++;
    }

    // add the item to the back
    public void addLast(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot add a null object to the end of the deque!");
        }

        Node oldLast = last_;
        last_ = new Node();
        last_.item = item;

        if (size_ == 0) {
            first_ = last_;
        } else {
            last_.next = oldLast;
            oldLast.previous = last_;
        }
        size_++;
    }

    // remove and return the item from the front
    public Item removeFirst() {
        if (size_ == 0) {
            throw new java.util.NoSuchElementException("Cannot remove the first node from an empty deque!");
        }

        Item itemInFirst = first_.item;

        // Set the next method of the previous node to point to null
        first_ = first_.previous;
        if (first_ != null) {
            first_.next = null;
        } else {
            last_ = null;
        }
        size_--;
        return itemInFirst;
    }

    // remove and return the item from the back
    public Item removeLast() {
        if (size_ == 0) {
            throw new java.util.NoSuchElementException("Cannot remove the last node from an empty deque!");
        }

        Item itemInLast = last_.item;

        // Set the previous method of the next node to point to null
        last_ = last_.next;
        if (last_ != null) {
            last_.previous = null;
        } else {
            first_ = null;
        }
        size_--;
        return itemInLast;
    }

    // return an iterator over items in order from front to back
    public Iterator<Item> iterator() {
        return new DequeIterator();
    }

    // unit testing (required)
    public static void main(String[] args) {
    }
}
