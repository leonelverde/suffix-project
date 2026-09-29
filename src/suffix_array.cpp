#include "suffix_array.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>

SuffixArray::SuffixArray(const std::string& t) : text(t) {
    buildSA();
    buildLCP();
}

void SuffixArray::buildSA() {
    const int n = static_cast<int>(text.size());
    sa.assign(n, 0);
    rank.assign(n, 0);
    if (n == 0) return;

    std::iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) rank[i] = static_cast<unsigned char>(text[i]);
    std::vector<int> tmp(n);

    // Each round sorts the suffixes by their first 2k characters.
    for (int k = 1;; k <<= 1) {
        auto less = [&](int a, int b) {
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            int ra = (a + k < n) ? rank[a + k] : -1;
            int rb = (b + k < n) ? rank[b + k] : -1;
            return ra < rb;
        };

        std::sort(sa.begin(), sa.end(), less);

        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            tmp[sa[i]] = tmp[sa[i - 1]] + (less(sa[i - 1], sa[i]) ? 1 : 0);
        rank = tmp;

        if (rank[sa[n - 1]] == n - 1) break;  // all ranks are distinct
    }
}

void SuffixArray::buildLCP() {
    const int n = static_cast<int>(text.size());
    lcp.assign(n, 0);

    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank[i] > 0) {
            int j = sa[rank[i] - 1];  // previous suffix in sorted order
            while (i + h < n && j + h < n && text[i + h] == text[j + h]) ++h;
            lcp[rank[i]] = h;
            if (h > 0) --h;
        } else {
            h = 0;
        }
    }
}

SearchRange SuffixArray::searchRange(const std::string& pattern) const {
    const int n = static_cast<int>(sa.size());
    const size_t m = pattern.size();
    if (m == 0 || n == 0) return SearchRange();

    // Binary search 1: leftmost position.
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (text.compare(sa[mid], m, pattern) < 0) lo = mid + 1;
        else hi = mid;
    }
    int left = lo;

    // Binary search 2: first position after the last match.
    hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (text.compare(sa[mid], m, pattern) <= 0) lo = mid + 1;
        else hi = mid;
    }
    return SearchRange(left, lo);
}

std::vector<int> SuffixArray::search(const std::string& pattern) const {
    std::vector<int> positions;
    SearchRange range = searchRange(pattern);
    for (int i = range.left; i < range.right; ++i) positions.push_back(sa[i]);
    std::sort(positions.begin(), positions.end());
    return positions;
}

std::string SuffixArray::longestRepeatedSubstring() const {
    int best = 0, index = 0;
    for (int i = 1; i < static_cast<int>(lcp.size()); ++i) {
        if (lcp[i] > best) {
            best = lcp[i];
            index = i;
        }
    }
    if (best == 0) return "";
    return text.substr(sa[index], best);
}

void SuffixArray::print() const {
    std::cout << std::setw(4) << "i" << std::setw(8) << "SA[i]"
              << std::setw(8) << "LCP[i]" << "  sufijo\n";
    for (int i = 0; i < static_cast<int>(sa.size()); ++i) {
        std::cout << std::setw(4) << i << std::setw(8) << sa[i]
                  << std::setw(8) << lcp[i] << "  " << text.substr(sa[i]) << "\n";
    }
}