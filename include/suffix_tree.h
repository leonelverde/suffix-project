#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <map>
#include <string>
#include <vector>

// Suffix tree node. Each edge is labeled with text[start, end).
struct SuffixNode {
    int start;
    int end;
    int suffixIndex;                     // only for leaves; -1 for internal nodes
    std::map<char, SuffixNode*> children;

    SuffixNode(int s = 0, int e = 0, int idx = -1)
        : start(s), end(e), suffixIndex(idx) {}

    bool isLeaf() const { return children.empty(); }
};

// Suffix tree built naively: O(m^2).
struct SuffixTree {
    std::string text;   // original text + '$' (unique terminator)
    SuffixNode* root;

    SuffixTree(const std::string& t);
    ~SuffixTree();
    SuffixTree(const SuffixTree&) = delete;
    SuffixTree& operator=(const SuffixTree&) = delete;

    void insertSuffix(int i);
    std::vector<int> search(const std::string& pattern) const;
    int countNodes() const;
    void print() const;
};

#endif