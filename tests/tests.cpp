#include "Compress.h"
#include "Decompress.h"
#include "BitWriter.h"
#include "Trie.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <sstream>

void CheckOutput(const std::vector<uint16_t>& output, const std::vector<char>& input) {
    size_t index_bit = 0, jndex_bit = 0;
    size_t index = 0, jndex = 0;

    while (index < output.size()) {
        REQUIRE(jndex < input.size());

        int bit_output = (output[index] >> (9 - index_bit - 1) & 1);
        int bit_input = (input[jndex] >> (8 - jndex_bit - 1) & 1);

        REQUIRE(bit_input == bit_output);

        ++index_bit;
        ++jndex_bit;
        if (index_bit == 9) {
            ++index;
            index_bit = 0;
        }
        if (jndex_bit == 8) {
            ++jndex;
            jndex_bit = 0;
        }
    }
}

void EqualFiles(std::ifstream& tested, std::ifstream& correct) {
    while (!tested.eof() && !correct.eof()) {
        char ch1, ch2;
        tested.read(&ch1, 1);
        correct.read(&ch2, 1);

        REQUIRE(tested.eof() == correct.eof());

        if (tested.eof() && correct.eof()) {
            break;
        }

        REQUIRE(ch1 == ch2);
    }
}

TEST_CASE("TestBitReader") {
    std::vector<char> data;
    for (char ch = -128; ch < 127; ++ch) {
        data.push_back(ch);
    }

    std::stringstream stream;

    for (char ch : data) {
        stream.write(&ch, 1);
    }

    std::vector<uint16_t> reader_output;
    BitReader bit_reader(stream);

    for (size_t i = 0; i < data.size() * 8 / 9; ++i) {
        reader_output.push_back(bit_reader.Read9bits());
    }

    CheckOutput(reader_output, data);
}

TEST_CASE("TestBitWriter") {
    std::vector<uint16_t> data;
    for (uint16_t i = 0; i < (1 << 9); ++i) {
        data.push_back(i);
    }

    std::stringstream stream;
    BitWriter bit_writer(stream);

    for (uint16_t value : data) {
        bit_writer.Write9bits(value);
    }
    bit_writer.Flush();

    std::vector<char> writer_output(data.size() * 9 / 8);

    for (size_t i = 0; i < data.size() * 9 / 8; ++i) {
        stream.read(&writer_output[i], 1);
    }

    CheckOutput(data, writer_output);
}

TEST_CASE("TestTrie") {
    std::vector<Symbol> alphabet = {
            {0, 0, 1},
            {1, 2, 2},
            {2, 3, 2}
    };
    Trie trie1(alphabet);

    std::vector<Symbol> codes = trie1.GetCodes();

    REQUIRE(alphabet.size() == codes.size());

    for (size_t i = 0; i < alphabet.size(); ++i) {
        REQUIRE(alphabet[i].code_len == codes[i].code_len);
        REQUIRE(alphabet[i].code == codes[i].code);
        REQUIRE(alphabet[i].value == codes[i].value);
    }

    // "0 10 11" = 01011 = 01011000 = 8 + 16 + 64 = 88
    std::stringstream stream;
    char ch = 88;
    stream.write(&ch, 1);

    BitReader bit_reader(stream);

    REQUIRE(trie1.GetNextValueInStream(bit_reader) == 0);
    REQUIRE(trie1.GetNextValueInStream(bit_reader) == 1);
    REQUIRE(trie1.GetNextValueInStream(bit_reader) == 2);
}

TEST_CASE("TestCompress") {
    std::ifstream f_in("a", std::ios_base::binary);
    std::ofstream f_out("a.out", std::ios_base::binary);

    Compress("a", f_in, f_out, false);

    f_in.close();
    f_out.close();

    std::ifstream compress_result("a.out", std::ios_base::binary);
    std::ifstream correct_result("correct_a.out", std::ios_base::binary);

    EqualFiles(compress_result, correct_result);

    compress_result.close();
    correct_result.close();
}

TEST_CASE("TestDecompress") {
    std::ifstream f_in("b.out");
    Decompress(f_in);
    f_in.close();

    std::ifstream decompress_result("b");
    std::ifstream correct_result("correct_b");

    REQUIRE(decompress_result.is_open());

    EqualFiles(decompress_result, correct_result);

    decompress_result.close();
    correct_result.close();
}