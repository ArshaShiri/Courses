import java.util.HashMap;

public class Dictionary {
    // Dictionary implementation for words with upper case only.
    // Since there are 26 nodes and the upper case characters are
    // between 65 to 90 we need to subtract the ASCII code of each
    // character by 65.

    // *** *** *** *** *** Private Attributes *** *** *** *** *** //

    private static final int numberOfCharacters_ = 26;
    private DictionaryNode root_ = new DictionaryNode();
    private HashMap<Integer, String> wordNumToWord_;

    // *** *** *** *** *** Private Methods *** *** *** *** *** //

    private void put_(String key, Integer val) {
        root_ = put_(root_, key, val, 0);
    }

    private DictionaryNode put_(DictionaryNode x, String key, Integer val, int d) {
        if (x == null) x = new DictionaryNode();
        if (d == key.length()) {
            x.val_ = val;
            return x;
        }
        char c = key.charAt(d);
        x.next_[c - 65] = put_(x.next_[c - 65], key, val, d + 1);
        return x;
    }

    // *** *** *** *** *** Public Classes *** *** *** *** *** //

    public static class DictionaryNode {
        private Integer val_;
        private DictionaryNode[] next_ = new DictionaryNode[numberOfCharacters_];

        public DictionaryNode[] next() {
            return next_;
        }

        public Integer val() {
            return val_;
        }
    }


    // *** *** *** *** *** Public Methods *** *** *** *** *** //

    String getWordFromNumber(Integer num) {
        return wordNumToWord_.get(num);
    }

    static int getNumberOfCharacters() {
        return numberOfCharacters_;
    }

    DictionaryNode getRoot_() {
        return root_;
    }

    boolean isWordInDictionary(String word) {
        Dictionary.DictionaryNode dicNode = root_;
        for (int character = 0; character < word.length(); character++) {
            char currentCharacter = word.charAt(character);
            dicNode = dicNode.next()[currentCharacter - 65];
            if (dicNode == null) return false;
            if ((dicNode.val() != null) && (character == word.length() - 1)) return true;
        }
        return false;
    }

    public Dictionary(String[] dictionary) {
        if (dictionary == null) {
            throw new IllegalArgumentException("File is null!");
        }

        int counter = 0;
        wordNumToWord_ = new HashMap<>();
        for (String word : dictionary) {
            if (word.length() > 2) {
                put_(word, counter);
                wordNumToWord_.put(counter, word);
                counter++;
            }
        }
    }
}
