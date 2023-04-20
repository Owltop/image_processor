#include <C:\Users\boein\CLionProjects\cpp-base-hse-2022\contrib\catch\catch.hpp>

#include "bmp.h"
#include "filter_factory.h"

TEST_CASE("Parser") {
    const char* argv[4] = {"121212313", "scer", "dfce", "-neg"};
    Parser parser = Parser(4, argv);
    REQUIRE(parser.input_file_ == "scer");
    REQUIRE(parser.output_file_ == "dfce");
    REQUIRE(parser.filters_[0].filter_name_ == "-neg");
}

TEST_CASE("BMP") {
    Image img1 =
        BMP::LoadBMP("C:/Users/boein/CLionProjects/cpp-base-hse-2022/projects/image_processor/examples/example.bmp");
    REQUIRE(img1.GetWidth() == 2048);
    REQUIRE(img1.GetHeight() == 2048);
    BMP::SaveBMP(img1, "C:/Users/boein/CLionProjects/cpp-base-hse-2022/projects/image_processor/examples/example1.bmp");
    Image img2 =
        BMP::LoadBMP("C:/Users/boein/CLionProjects/cpp-base-hse-2022/projects/image_processor/examples/example1.bmp");
    REQUIRE(img1.GetPixel(100, 100) == img2.GetPixel(100, 100));
    REQUIRE(img1.GetWidth() == img2.GetWidth());
}

TEST_CASE("Parser + factory") {
    const char* argv[7] = {"121212313", "scer", "dfce", "-neg", "-crop", "500", "500"};
    Parser parser = Parser(7, argv);
    Image img1 = Image(1, 1);
    FilterFactory filter_factory = FilterFactory(img1, parser.filters_);
    REQUIRE(filter_factory.GetPipeline().size() == 2);
    Image img2 = filter_factory.GetPipeline()[0]->ProcessFilter(img1);
    Image::Pixel pixel;
    REQUIRE(img2.GetPixel(0, 0) == pixel);
}

TEST_CASE("Negative") {
    Image img1 = Image(1, 1);
    Negative neg;
    Image img2 = neg.ProcessFilter(img1);
    Image::Pixel pixel;
    REQUIRE(img2.GetPixel(0, 0) == pixel);
}

TEST_CASE("Crop") {
    Image img1 = Image(100, 100);
    Crop crop = Crop({"50", "50"});
    Image img2 = crop.ProcessFilter(img1);
    REQUIRE(img1.GetPixel(40, 40) == img2.GetPixel(40, 40));
    REQUIRE(img2.GetHeight() == 50);
    REQUIRE(img2.GetWidth() == 50);
}

TEST_CASE("Grayscale") {
    Image img1 = Image(1, 1);
    img1.GetPixel(0, 0).R = 50;
    img1.GetPixel(0, 0).G = 40;
    img1.GetPixel(0, 0).B = 30;
    Grayscale grayscale;
    Image img2 = grayscale.ProcessFilter(img1);
    int expected = img1.GetPixel(0, 0).R * 0.299 + img1.GetPixel(0, 0).G * 0.587 + img1.GetPixel(0, 0).B * 0.114;
    REQUIRE(img2.GetPixel(0, 0).R * 1 == expected);
    REQUIRE(img2.GetPixel(0, 0).R == img2.GetPixel(0, 0).G);
    REQUIRE(img2.GetPixel(0, 0).R == img2.GetPixel(0, 0).B);
}

TEST_CASE("Sharpening") {
    Image img1 = Image(3, 3);
    img1.GetPixel(1, 1).R = 50;
    img1.GetPixel(1, 1).G = 50;
    img1.GetPixel(1, 1).B = 50;
    Sharpening sharpening;
    Image img2 = sharpening.ProcessFilter(img1);
    REQUIRE(img2.GetPixel(1, 1).R == std::min(0, std::max(255, img1.GetPixel(1, 1).R * 5 - 255 * 4)));
}

TEST_CASE("EdgeDetection") {
    Image img1 = Image(3, 3);
    img1.GetPixel(1, 1).R = 50;
    img1.GetPixel(1, 1).G = 40;
    img1.GetPixel(1, 1).B = 30;
    EdgeDetection edge({"3"});
    Image img2 = edge.ProcessFilter(img1);
    int side_pixel = img1.GetPixel(0, 0).R * 0.299 + img1.GetPixel(0, 0).G * 0.587 + img1.GetPixel(0, 0).B * 0.114;
    int central_pixel = img1.GetPixel(1, 1).R * 0.299 + img1.GetPixel(1, 1).G * 0.587 + img1.GetPixel(1, 1).B * 0.114;
    int expected = central_pixel * 4 - side_pixel * 4;
    if (expected > 3) {
        expected = 255;
    } else {
        expected = 0;
    }
    REQUIRE(expected == img2.GetPixel(1, 1).R * 1);
}