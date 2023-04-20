#include "bmp.h"
#include "filter_factory.h"

int main(int argc, char* argv[]) {
    try {
        Parser parser = Parser(argc, argv);
        auto image = BMP::LoadBMP(parser.input_file_);
        FilterFactory filter_factory(image, parser.filters_);
        Image result = image;
        for (const auto& filter : filter_factory.GetPipeline()) {
            result = filter->ProcessFilter(result);
        }
        BMP::SaveBMP(result, parser.output_file_);
        return 0;
    } catch (const CustomExceptions& e) {
        std::cout << e.message << std::endl;
        return 1;
    }
}
