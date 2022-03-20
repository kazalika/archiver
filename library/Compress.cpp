//#pragma once

#include <set>
#include <iostream>
#include "Trie.h"
#include "BitWriter.h"
#include "Compress.h"

uint16_t GetNextValue(std::istream &f) {
    char ch;
    f.read(&ch, 1);
    unsigned char uch = static_cast<unsigned char>(ch);
    return static_cast<uint16_t>(uch);
}

NodePtr MergeNodePtr(NodePtr left, NodePtr right) {
    return std::make_shared<Node>(left, right, left->freq + right->freq);
}

struct Compare {
    bool operator() (const NodePtr& a, const NodePtr& b) const {
        return std::tie(a->freq, a->value, a) < std::tie(b->freq, b->value, b);
    }
};

NodePtr GetFirstElement(std::set<NodePtr, Compare>& set) {
    NodePtr first_element = *(set.begin());
    set.erase(set.begin());
    return first_element;
}

void Compress(const char* name_of_file, std::ifstream& f_in, std::ofstream& f_out, bool is_next_file) {
    if (!f_in.is_open()) {
        std::cerr << "File " << name_of_file << " was not found\n";
        throw;
    }

    int counter[ALPHABET_SIZE]{};
    std::string str_name_of_file(name_of_file);

    for (char ch : str_name_of_file) {
        uint16_t value = static_cast<uint16_t>(ch);
        ++counter[value];
    }
    while (!f_in.eof()) {
        uint16_t value = GetNextValue(f_in);
        if (!f_in.eof()) {
            ++counter[value];
        }
    }

    counter[FILENAME_END] = counter[ONE_MORE_FILE] = counter[ARCHIVE_END] = 1;

    std::set<NodePtr, Compare> node_queue;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (counter[i] > 0) {
            NodePtr cur = std::make_shared<Node>(i, counter[i]);
            node_queue.insert(cur);
        }
    }

    while (node_queue.size() > 1) {
        NodePtr first = GetFirstElement(node_queue);
        NodePtr second = GetFirstElement(node_queue);

        NodePtr cur = MergeNodePtr(first, second);
        node_queue.insert(cur);
    }

    Trie trie(GetFirstElement(node_queue));

    std::vector<Symbol> codes = trie.GetCodes();
    std::map<uint16_t, size_t> val_to_index;

    MakeCanonicalCodes(codes);

    uint32_t symbols_count = codes.size(), max_code_len = 0;
    int counter_len[ALPHABET_SIZE]{};
    
    BitWriter out(f_out);

    // SYMBOLS_COUNT
    out.Write9bits(symbols_count);

    // SYMBOLS
    for (size_t i = 0; i < codes.size(); ++i) {
        Symbol &sym = codes[i];
        out.Write9bits(sym.value);
        max_code_len = std::max(max_code_len, static_cast<uint32_t>(sym.code_len));
        ++counter_len[sym.code_len];
        val_to_index[sym.value] = i;
    }

    if (max_code_len > 64) {
        std::cerr << "symbol's code is too long: " << max_code_len << '\n';
        throw;
    }

    // CODES_LENS
    for (size_t i = 1; i <= max_code_len; ++i) {
        out.Write9bits(counter_len[i]);
    }

    auto write_code = [&codes, &val_to_index, &out](uint16_t value) {
        Symbol &sym = codes[val_to_index[value]];
        for (size_t i = 0; i < sym.code_len; ++i) {
            uint64_t bit = (sym.code >> (sym.code_len - i - 1) & 1);
            out.Write1bit(bit);
        }
    };

    // FILE_NAME
    for (char ch : str_name_of_file) {
        write_code(static_cast<uint16_t>(ch));
    }

    // FILENAME_END
    write_code(FILENAME_END);

    // FILE_DATA
    f_in.clear();
    f_in.seekg(0);
    while (!f_in.eof()) {
        uint16_t value = GetNextValue(f_in);
        if (!f_in.eof()) {
            write_code(value);
        }
    }

    // ONE_MORE_FILE or ARCHIVE_END
    if (is_next_file) {
        write_code(ONE_MORE_FILE);
    } else {
        write_code(ARCHIVE_END);
    }

    out.Flush();

}