#include <iostream>
#include <string_view>
#include <vector>
#include <fstream>

class BitReader {
public:
    BitReader(std::istream &fin);

    uint16_t GetNextByte();

    uint16_t Read9bits();

    uint16_t Read1bit();

    void Flush();

private:
    std::istream &fin_;
    uint16_t first_ = 0, second_ = 0, pointer_ = 0;
};