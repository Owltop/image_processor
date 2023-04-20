#pragma once

#include "image.h"
#include <string>

static const std::vector<std::string> DEFAULT_MATRIX = {"0", "0", "0", "0", "1"};
static const std::vector<std::string> SHARPENING_MATRIX = {"-1", "-1", "-1", "-1", "5"};
static const std::vector<std::string> EDGE_MATRIX = {"-1", "-1", "-1", "-1", "4"};

class BaseFilter {
public:
    BaseFilter(std::vector<std::string> v42 = {}) : params(v42){};
    virtual Image ProcessFilter(Image& image) = 0;

protected:
    std::vector<std::string> params = {};
};

class MatrixFilter : public BaseFilter {
public:
    MatrixFilter(std::vector<std::string> v42 = DEFAULT_MATRIX) : BaseFilter(v42){};
    Image ProcessFilter(Image& image) override;
};

class Crop : public BaseFilter {
public:
    using BaseFilter::BaseFilter;
    Image ProcessFilter(Image& image) override;
};

class Grayscale : public BaseFilter {
public:
    using BaseFilter::BaseFilter;
    Image ProcessFilter(Image& image) override;
};

class Negative : public BaseFilter {
public:
    using BaseFilter::BaseFilter;
    Image ProcessFilter(Image& image) override;
};

class Sharpening : public MatrixFilter {
public:
    Sharpening(std::vector<std::string> v42 = {}) : MatrixFilter(SHARPENING_MATRIX){};
};

class EdgeDetection : public Grayscale {
public:
    EdgeDetection(std::vector<std::string> params = {"3"});
    Image ProcessFilter(Image& image) override;

protected:
    int threshold = 0;
};

class GaussianBlurFilter : public BaseFilter {
public:
    using BaseFilter::BaseFilter;
    Image ProcessFilter(Image& result_image) override;
    void FillMatrixCoefficients(int sigma);

protected:
    size_t gauss_size_ = 0;
    std::vector<double> gauss_coefficients_;
    double coefficients_sum_ = 0;
};

class TrianglesFilter : public BaseFilter {
public:
    using BaseFilter::BaseFilter;
    Image ProcessFilter(Image& image) override;

protected:
    std::vector<std::string> params = {"30"};
    Image::Pixel Average(const std::vector<Image::Pixel>& v);  // стандартные функции не работали
};
