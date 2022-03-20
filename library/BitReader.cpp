#include "BitReader.h"

BitReader::BitReader(std::istream &fin) : fin_(fin) {
}

uint16_t BitReader::GetNextByte() {
    char in_char;
    fin_.read(&in_char, 1);
    if (fin_.eof()) {
        std::cerr << "Unexpected end of file in BitReader\n";
        throw std::exception();
    }
    unsigned char uin_char = static_cast<unsigned char>(in_char);
    return static_cast<uint16_t>(uin_char);
}

uint16_t BitReader::Read9bits() {
    uint16_t result = 0;
    if (pointer_ == 0) {
        first_ = GetNextByte();
        second_ = GetNextByte();
        result = (first_ << 1) | (second_ >> 7);
        first_ = (second_ & ((1 << 7) - 1)) << 1;
        second_ = 0;
        pointer_ = 7;
    } else {
        second_ = GetNextByte();
        result = (first_ << 1) | (second_ >> (pointer_ - 1));
        first_ = (second_ & ((1 << (pointer_ - 1)) - 1)) << (8 - (pointer_ - 1));
        second_ = 0;
        --pointer_;
    }
    return result;
}

void BitReader::Flush() {
    first_ = second_ = 0;
    pointer_ = 0;
}

uint16_t BitReader::Read1bit() {
    uint16_t result = 0;
    if (pointer_ == 0) {
        first_ = GetNextByte();
        pointer_ = 8;
    }
    result = first_ >> 7 & 1;
    --pointer_;
    first_ <<= 1;
    first_ &= (1 << 8) - 1;
    return result;
}
