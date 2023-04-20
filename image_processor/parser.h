#pragma once
#include <string_view>
#include <vector>
#include <iostream>
#include "struct_filter.h"

class Parser {
public:
    Parser(int argc, const char* argv[]);
    Parser(int argc, char* argv[]);

    std::string input_file_;
    std::string output_file_;
    std::vector<Filter> filters_;
};