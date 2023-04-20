#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

struct CustomExceptions : std::exception {
    explicit CustomExceptions(const std::string& mess) : message(mess){};
    std::string message;
};

class Image {
public:
    struct Pixel {
        uint8_t R = 0;
        uint8_t G = 0;
        uint8_t B = 0;
        Pixel operator+(const Pixel& another_pixel) const;
        Pixel operator/(int a) const;
        bool operator==(const Pixel& other) const;
    };

    Image(int w, int h);
    Pixel& GetPixel(int x, int y);
    Pixel GetPixel(int x, int y) const;
    size_t GetWidth() const;
    size_t GetHeight() const;

private:
    std::vector<std::vector<Pixel>> image_;
};