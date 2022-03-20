#include <iostream>
#include <string_view>
#include <vector>
#include <fstream>

class BitWriter {
public:

    BitWriter(std::ostream &fout);

    void WriteNextByte(char value);

    void Write9bits(uint16_t value);

    void Write1bit(uint16_t value);

    void Flush();

private:
    std::ostream &fout_;
    uint16_t first_ = 0, second_ = 0, pointer_ = 0;
};
