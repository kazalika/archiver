//#pragma once

#include <fstream>
#include "Decompress.h"
#include "Trie.h"


void Decompress(std::ifstream& f_in) {
    if (!f_in.is_open()) {
        std::cerr << "Failed to open file in Decompress\n";
        throw std::exception();
    }

    f_in.clear();
    f_in.seekg(0);

    BitReader in(f_in);

    bool is_one_more_file = true;

    while (is_one_more_file) {
        uint16_t symbols_count = in.Read9bits();
        std::vector<Symbol> alphabet(symbols_count);
        for (Symbol &sym : alphabet) {
            sym.value = in.Read9bits();
        }

        uint16_t all_symbol_counter = 0;
        for (size_t i = 1, j = 0; all_symbol_counter != symbols_count; ++i) {
            uint16_t current_symbol_count = in.Read9bits();
            all_symbol_counter += current_symbol_count;
            for (size_t k = 0; k < current_symbol_count; ++k, ++j) {
                alphabet[j].code_len = i;
            }
        }

        MakeCanonicalCodes(alphabet);

        Trie trie(alphabet);

        uint16_t last_read_value = 0;
        std::string file_name;
        while (last_read_value != FILENAME_END) {
            last_read_value = trie.GetNextValueInStream(in);
            if (last_read_value != FILENAME_END) {
                file_name += static_cast<char>(last_read_value);
            }
        }

        std::ofstream f_out(file_name.data());

        last_read_value = 0;
        while (last_read_value != ONE_MORE_FILE && last_read_value != ARCHIVE_END) {
            last_read_value = trie.GetNextValueInStream(in);
            if (last_read_value != ONE_MORE_FILE && last_read_value != ARCHIVE_END) {
                char out_ch = static_cast<char>(last_read_value);
                f_out.write(&out_ch, 1);
            }
        }

        f_out.close();
        in.Flush();

        if (last_read_value == ARCHIVE_END) {
            is_one_more_file = false;
        }
    }
}