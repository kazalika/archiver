#include <iostream>
#include <string>
#include "Compress.h"
#include "Decompress.h"

const std::string HELP_TEXT = "Usage\n"
                              "\n"
                              "  archiver -c archive_name file1 [file2 ...]\n"
                              "  archiver -d archive_name\n"
                              "  archiver -h\n"
                              "\n"
                              "Options\n"
                              "  -c archive_name file1 [file2 ...]      = compress file1 [file2 ...] to archive_name.\n"
                              "  -d archive_name                        = decompress archive_name to original files.\n"
                              "  -h                                     = print usage information and exit.\n";

const std::string ARGUMENTS_ERROR = "Need arguments\nRun 'archiver -h' for more information";

void Zip(char* archive_name, char* files[], int count_of_files) {
    std::ofstream f_out(archive_name, std::ios_base::binary);
    for (int i = 0; i < count_of_files; ++i) {
        std::ifstream f_in(files[i], std::ios_base::binary);
        Compress(files[i], f_in, f_out, i + 1 != count_of_files);
        f_in.close();
    }
    f_out.close();
}

void Unzip(char* archive_name) {
    std::ifstream f_in(archive_name, std::ios_base::binary);
    Decompress(f_in);
    f_in.close();
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        throw std::invalid_argument(ARGUMENTS_ERROR);
    }

    std::string type_of_command(argv[1]);

    if (type_of_command == "-c") {
        if (argc <= 3) {
            throw std::invalid_argument(ARGUMENTS_ERROR);
        }
        Zip(argv[2], argv + 3, argc - 3);
    }
    else if (type_of_command == "-d") {
        if (argc <= 2) {
            throw std::invalid_argument(ARGUMENTS_ERROR);
        }
        Unzip(argv[2]);
    }
    else if (type_of_command == "-h") {
        std::cout << HELP_TEXT;
    } else {
        throw std::invalid_argument("Didn't found this type of option\nRun 'archiver -h' for more information\"");
    }

    return 0;
}
