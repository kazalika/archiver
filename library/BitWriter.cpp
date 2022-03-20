#include "BitWriter.h"

BitWriter::BitWriter(std::ostream &fout) : fout_(fout) {
}

void BitWriter::WriteNextByte(char value) {
//    char ch = static_cast<char>(value);
    fout_.write(&value, 1);
}

void BitWriter::Write9bits(uint16_t value) {
    first_ |= value >> (pointer_ + 1);
    second_ = (value & ((1 << (pointer_ + 1)) - 1)) << (7 - pointer_);
    WriteNextByte(first_);
    first_ = second_;
    second_ = 0;
    if (pointer_ == 7) {
        WriteNextByte(first_);
        first_ = second_;
    }
    pointer_ = (pointer_ + 1) % 8;
}

void BitWriter::Write1bit(uint16_t value) {
    first_ |= value << (7 - pointer_);
    if (pointer_ == 7) {
        WriteNextByte(first_);
        first_ = 0;
        pointer_ = 0;
    } else {
        ++pointer_;
    }
}

void BitWriter::Flush() {
    if (pointer_ > 0) {
        WriteNextByte(first_);
    }
    first_ = 0;
    pointer_ = 0;
}
