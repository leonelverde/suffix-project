#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <string>
#include <vector>

// Interval [left, right) inside the suffix array.
struct SearchRange {
    int left;
    int right;

    SearchRange(int l = 0, int r = 0) : left(l), right(r) {}

    bool empty() const { return left >= right; }
    int size() const { return right - left; }
};

struct SuffixArray {
    std::string text;
    std::vector<int> sa;    // sa[i]   = start of the i-th suffix in lexicographic order
    std::vector<int> rank;  // rank[p] = position in sa of the suffix starting at p
    std::vector<int> lcp;   // lcp[i]  = lcp(sa[i-1], sa[i]); lcp[0] = 0

    SuffixArray(const std::string& t);

    void buildSA();   // prefix doubling: O(n log^2 n)
    void buildLCP();  // Kasai's algorithm: O(n)

    SearchRange searchRange(const std::string& pattern) const;  // 2 binary searches
    std::vector<int> search(const std::string& pattern) const;
    std::string longestRepeatedSubstring() const;
    void print() const;
};

#endif