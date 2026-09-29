#include "suffix_tree.h"

#include <algorithm>
#include <iostream>
#include <iterator>

static void freeNode(SuffixNode* node) {
    for (auto& entry : node->children) freeNode(entry.second);
    delete node;
}

static void collectLeaves(const SuffixNode* node, std::vector<int>& result) {
    if (node->isLeaf()) {
        result.push_back(node->suffixIndex);
        return;
    }
    for (const auto& entry : node->children) collectLeaves(entry.second, result);
}

static int countRec(const SuffixNode* node) {
    int total = 1;
    for (const auto& entry : node->children) total += countRec(entry.second);
    return total;
}

static void printRec(const std::string& text, const SuffixNode* node,
                     const std::string& prefix) {
    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
        bool isLast = (std::next(it) == node->children.end());
        const SuffixNode* child = it->second;
        std::cout << prefix << (isLast ? "+-- " : "|-- ")
                  << text.substr(child->start, child->end - child->start);
        if (child->isLeaf()) std::cout << "  [sufijo " << child->suffixIndex << "]";
        std::cout << "\n";
        printRec(text, child, prefix + (isLast ? "    " : "|   "));
    }
}

SuffixTree::SuffixTree(const std::string& t)
    : text(t + '$'), root(new SuffixNode()) {
    for (int i = 0; i < static_cast<int>(text.size()); ++i) insertSuffix(i);
}

SuffixTree::~SuffixTree() { freeNode(root); }

void SuffixTree::insertSuffix(int i) {
    const int n = static_cast<int>(text.size());
    SuffixNode* node = root;
    int pos = i;

    while (true) {
        char c = text[pos];
        auto it = node->children.find(c);

        // No edge starts with c: create a new leaf.
        if (it == node->children.end()) {
            node->children[c] = new SuffixNode(pos, n, i);
            return;
        }

        SuffixNode* child = it->second;
        int length = child->end - child->start;
        int k = 0;
        while (k < length && text[child->start + k] == text[pos + k]) ++k;

        // The whole edge matches: keep going down.
        if (k == length) {
            node = child;
            pos += k;
            continue;
        }

        // Only part of the edge matches: split it and add the new leaf.
        SuffixNode* middle = new SuffixNode(child->start, child->start + k);
        child->start += k;
        middle->children[text[child->start]] = child;
        middle->children[text[pos + k]] = new SuffixNode(pos + k, n, i);
        node->children[c] = middle;
        return;
    }
}

std::vector<int> SuffixTree::search(const std::string& pattern) const {
    std::vector<int> positions;
    const int m = static_cast<int>(pattern.size());
    if (m == 0) return positions;

    const SuffixNode* node = root;
    int pos = 0;
    while (pos < m) {
        auto it = node->children.find(pattern[pos]);
        if (it == node->children.end()) return positions;

        const SuffixNode* child = it->second;
        int k = child->start;
        while (k < child->end && pos < m) {
            if (text[k] != pattern[pos]) return positions;
            ++k;
            ++pos;
        }
        node = child;
    }

    // Every leaf below this point marks a starting position of the pattern.
    collectLeaves(node, positions);
    std::sort(positions.begin(), positions.end());
    return positions;
}

int SuffixTree::countNodes() const { return countRec(root); }

void SuffixTree::print() const {
    std::cout << "(raiz)\n";
    printRec(text, root, "");
}