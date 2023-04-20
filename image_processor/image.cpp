#include "image.h"
Image::Image(int w, int h) {
    image_.resize(h, std::vector<Pixel>(w, {.R = 255, .G = 255, .B = 255}));
}

Image::Pixel& Image::GetPixel(int x, int y) {
    return image_[x][y];
}

Image::Pixel Image::GetPixel(int x, int y) const {
    return image_[x][y];
}

size_t Image::GetHeight() const {
    return image_.size();
}

size_t Image::GetWidth() const {
    return image_.front().size();
}

Image::Pixel Image::Pixel::operator+(const Image::Pixel& another_pixel) const {
    Pixel result;
    result.R = this->R + another_pixel.R;
    result.G = this->G + another_pixel.G;
    result.B = this->B + another_pixel.B;
    return result;
}

Image::Pixel Image::Pixel::operator/(int a) const {
    Pixel result;
    result.R = this->R / a;  //
    result.G = this->G / a;
    result.B = this->B / a;
    return result;
}
bool Image::Pixel::operator==(const Pixel& other) const {
    if (this->R == other.R && this->G == other.G && this->B == other.B) {
        return true;
    }
    return false;
}

// Image::Pixel& Image::Pixel::operator=(const Image::Pixel& another_pixel) {
//     this->R = another_pixel.R;
//     this->G = another_pixel.G;
//     this->B = another_pixel.B;
//     return *this;
// }
