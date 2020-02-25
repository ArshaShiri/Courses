import edu.princeton.cs.algs4.StdRandom;

import java.util.Iterator;

public class RandomizedQueue<Item> implements Iterable<Item> {
    // *** *** *** *** *** Private attributes *** *** *** *** *** //

    private Item[] randomQueue_;
    private int size_ = 0;

    // *** *** *** *** *** Private methods *** *** *** *** *** //

    private void resize_(int newSize) {
        //System.out.println("resize is called to change the size from " + randomQueue.length + " to " + newSize);

        Item[] newRandomQueue = (Item[]) new Object[newSize];
        if (size_ >= 0) System.arraycopy(randomQueue_, 0, newRandomQueue, 0, size_);
        randomQueue_ = newRandomQueue;
    }

    // *** *** *** *** *** Private classes *** *** *** *** *** //
    private class RandomizedQueueIterator implements Iterator<Item> {
        public RandomizedQueueIterator() {
            shuffledIndices = new int[size_];

            for (int index = 0; index < size_; ++index) {
                shuffledIndices[index] = index;
            }
            StdRandom.shuffle(shuffledIndices);
        }

        public boolean hasNext() {
            return currentIndex < size_;
        }

        public void remove() {
            throw new UnsupportedOperationException();
        }

        public Item next() {
            if (!hasNext()) {
                throw new java.util.NoSuchElementException("Next item does not exist in the iterator! ");
            }

            return randomQueue_[shuffledIndices[currentIndex++]];
        }

        private int currentIndex = 0;
        private int[] shuffledIndices;
    }

    // *** *** *** *** *** Public methods *** *** *** *** *** //

    // construct an empty randomized queue
    public RandomizedQueue() {
    }

    // is the randomized queue empty?
    public boolean isEmpty() {
        return size_ == 0;
    }

    // return the number of items on the randomized queue
    public int size() {
        return size_;
    }

    // add the item
    public void enqueue(Item item) {
        if (item == null) {
            throw new IllegalArgumentException("Cannot add a null object to the random queue!");
        }

        // Initialize the queue if it is empty
        if (size_ == 0) {
            randomQueue_ = (Item[]) new Object[1];
        }

        if (size_ == randomQueue_.length) {
            this.resize_(2 * size_);
        }

        randomQueue_[size_++] = item;
    }

    // remove and return a random item
    public Item dequeue() {
        if (size_ == 0) {
            throw new java.util.NoSuchElementException("Cannot dequeue an empty random queue!");
        }

        int randomIndex = StdRandom.uniform(size_);
        Item itemAtRandomIndex = randomQueue_[randomIndex];
        randomQueue_[randomIndex] = randomQueue_[size_ - 1];
        randomQueue_[size_ - 1] = null;
        size_--;

        if (size_ == randomQueue_.length / 4) {
            this.resize_(randomQueue_.length / 2);
        }

        return itemAtRandomIndex;
    }

    // return a random item (but do not remove it)
    public Item sample() {
        if (size_ == 0) {
            throw new java.util.NoSuchElementException("Cannot dequeue an empty random queue!");
        }

        int randomIndex = StdRandom.uniform(size_);
        return randomQueue_[randomIndex];
    }

    // return an independent iterator over items in random order
    public Iterator<Item> iterator() {
        return new RandomizedQueueIterator();
    }

    // unit testing (required)
    public static void main(String[] args) {
        RandomizedQueue<Integer> rq = new RandomizedQueue<Integer>();
        rq.enqueue(983);
        System.out.println(rq.size());
        System.out.println(rq.sample());
        rq.size();
    }
}
