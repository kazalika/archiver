#pragma once

#include <cstdint>
#include <vector>

enum SERVICE_SYMBOLS {
    FILENAME_END = 256,
    ONE_MORE_FILE = 257,
    ARCHIVE_END = 258
};

class Symbol {
public:
    uint16_t value;
    uint64_t code;
    size_t code_len;
};

void MakeCanonicalCodes(std::vector<Symbol> &symbols);