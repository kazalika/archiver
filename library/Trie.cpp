#include "Trie.h"

void Trie::AddWord(const Symbol &symbol) {
    std::shared_ptr<Node> cur = root_;
    for (size_t i = 0; i < symbol.code_len; ++i) {
        if ((symbol.code >> (symbol.code_len - i - 1) & 1) == 1) {
            if (cur->r == nullptr) {
                cur->r = std::make_shared<Node>();
            }
            cur = cur->r;
        } else if ((symbol.code >> (symbol.code_len - i - 1) & 1) == 0) {
            if (cur->l == nullptr) {
                cur->l = std::make_shared<Node>();
            }
            cur = cur->l;
        }
    }
    cur->value = symbol.value;
    cur->term = true;
}

void Trie::TraverseTrie(NodePtr cur, uint64_t current_code, size_t current_code_len) {
    if (cur->term) {
        codes.push_back({cur->value, current_code, current_code_len});
        return;
    }
    TraverseTrie(cur->l, current_code << 1, current_code_len + 1);
    TraverseTrie(cur->r, current_code << 1 | 1, current_code_len + 1);
}



std::vector<Symbol> Trie::GetCodes() {
    codes.clear();
    TraverseTrie(root_, 0, 0);
    return codes;
}

NodePtr Trie::GetRoot() {
    return root_;
}

Trie::Trie(std::vector<Symbol> &alphabet)  {
    for (const Symbol &symbol : alphabet) {
        AddWord(symbol);
    }
}

Trie::Trie(NodePtr root) {
    root_ = root;
}

uint16_t Trie::GetNextValueInStream(BitReader &in) {
    NodePtr cur_node = root_;
    while (!cur_node->term) {
        uint16_t next_bit = in.Read1bit();
        if (next_bit == 1) {
            if (cur_node->r == nullptr) {
                std::cerr << "Unexpected code in compressed file\n";
                throw std::exception();
            }
            cur_node = cur_node->r;
        } else {
            if (cur_node->l == nullptr) {
                std::cerr << "Unexpected code in compressed file\n";
                throw std::exception();
            }
            cur_node = cur_node->l;
        }
    }
    return cur_node->value;
}