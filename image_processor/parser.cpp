#include "parser.h"

Parser::Parser(int argc, const char* argv[]) {  // для теста
    if (argc < 2) {  // 2 достаточно, просто переписали файл в новое место
        throw CustomExceptions("Not enough arguments");
    }
    try {
        for (int i = 1; i < argc; ++i) {
            if (i == 1) {
                input_file_ = argv[i];
            } else if (i == 2) {
                output_file_ = argv[i];
            } else {
                std::string current_argument = argv[i];
                if (current_argument[0] == '-') {
                    filters_.push_back(Filter());
                    filters_[size(filters_) - 1].filter_name_ = current_argument;
                } else {
                    filters_[size(filters_) - 1].params_.push_back(current_argument);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error with reading command line, check your filters and arguments" << std::endl;
    }
}

Parser::Parser(int argc, char* argv[]) {  // для проги
    if (argc < 2) {  // 2 достаточно, просто переписали файл в новое место
        throw CustomExceptions("Not enough arguments");
    }
    try {
        for (int i = 1; i < argc; ++i) {
            if (i == 1) {
                input_file_ = argv[i];
            } else if (i == 2) {
                output_file_ = argv[i];
            } else {
                std::string current_argument = argv[i];
                if (current_argument[0] == '-') {
                    filters_.push_back(Filter());
                    filters_[size(filters_) - 1].filter_name_ = current_argument;
                } else {
                    filters_[size(filters_) - 1].params_.push_back(current_argument);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error with reading command line, check your filters and arguments" << std::endl;
    }
}
