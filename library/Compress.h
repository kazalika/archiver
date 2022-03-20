#pragma once

#include <fstream>

void Compress(const char *name_of_file, std::ifstream &f_in, std::ofstream &f_out, bool is_next_file);