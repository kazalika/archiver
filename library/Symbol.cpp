#include "Symbol.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

void MakeCanonicalCodes(std::vector<Symbol> &symbols) {
    sort(symbols.begin(), symbols.end(), [](const Symbol &a, const Symbol &b) {
        return std::tie(a.code_len, a.value) < std::tie(b.code_len, b.value);
    });

    uint64_t current_code = 0;
    size_t current_code_len = 1;
    for (Symbol &sym : symbols) {
        if (current_code_len > sym.code_len) {
            std::cerr << "failed to get canonical codes\n";
            throw;
        }
        current_code <<= sym.code_len - current_code_len;
        sym.code = current_code;
        ++current_code;
        current_code_len = sym.code_len;
    }
}