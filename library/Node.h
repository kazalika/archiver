#pragma once

#include <cstdint>
#include <cstdio>
#include <tuple>
#include <bits/node_handle.h>
#include <memory>

const int ALPHABET_SIZE = 259;

class Node;

using NodePtr = std::shared_ptr<Node>;

class Node {
public:
    NodePtr l;
    NodePtr r;

    bool term;
    uint16_t value;
    size_t freq;

    Node() : l(nullptr), r(nullptr), term(false), value(0), freq(0) {}

    Node(uint16_t val) : l(nullptr), r(nullptr), term(true), value(val), freq(0) {}

    Node(NodePtr l, NodePtr r, size_t fr) : l(l), r(r), term(false), value(0), freq(fr) {}

    Node(uint16_t val, size_t fr) : l(nullptr), r(nullptr), term(true), value(val), freq(fr) {}
};
