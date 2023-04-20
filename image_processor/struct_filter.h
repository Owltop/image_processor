#include "image.h"

#include <string>
#include <vector>

struct Filter {
    std::string filter_name_;
    std::vector<std::string> params_;
};