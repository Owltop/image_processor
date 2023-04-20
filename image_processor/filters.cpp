#include "filters.h"

#include <cmath>
#include <iostream>

static const std::vector<std::pair<int, int>> NEIGHBOURS_SHIFT = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

Image MatrixFilter::ProcessFilter(Image &image) {
    Image result_image = Image(image.GetWidth(), image.GetHeight());
    int w = image.GetWidth();
    int h = image.GetHeight();
    for (int row = 0; row < h; ++row) {
        for (int column = 0; column < w; ++column) {
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            for (size_t i = 0; i < NEIGHBOURS_SHIFT.size(); ++i) {
                const auto &[dx, dy] = NEIGHBOURS_SHIFT[i];
                size_t new_x = std::max(0, std::min(h - 1, row + dx));
                size_t new_y = std::max(0, std::min(w - 1, column + dy));
                new_red += (std::stoi(params[i])) * static_cast<int>(image.GetPixel(new_x, new_y).R);
                new_green += (std::stoi(params[i])) * static_cast<int>(image.GetPixel(new_x, new_y).G);
                new_blue += (std::stoi(params[i])) * static_cast<int>(image.GetPixel(new_x, new_y).B);
            }
            new_red += std::stoi(params[params.size() - 1]) * static_cast<int>(image.GetPixel(row, column).R);
            new_green += std::stoi(params[params.size() - 1]) * static_cast<int>(image.GetPixel(row, column).G);
            new_blue += std::stoi(params[params.size() - 1]) * static_cast<int>(image.GetPixel(row, column).B);
            result_image.GetPixel(row, column).R = static_cast<uint8_t>(std::min(255, std::max(0, new_red)));
            result_image.GetPixel(row, column).G = static_cast<uint8_t>(std::min(255, std::max(0, new_green)));
            result_image.GetPixel(row, column).B = static_cast<uint8_t>(std::min(255, std::max(0, new_blue)));
        }
    }
    return result_image;
}

Image Grayscale::ProcessFilter(Image &image) {
    Image result_image = Image(image.GetWidth(), image.GetHeight());
    for (size_t row = 0; row < image.GetHeight(); ++row) {
        for (size_t column = 0; column < image.GetWidth(); ++column) {
            auto initial_color = image.GetPixel(row, column);
            result_image.GetPixel(row, column).R =
                0.299 * initial_color.R + 0.587 * initial_color.G + 0.114 * initial_color.B;
            result_image.GetPixel(row, column).G =
                0.299 * initial_color.R + 0.587 * initial_color.G + 0.114 * initial_color.B;
            result_image.GetPixel(row, column).B =
                0.299 * initial_color.R + 0.587 * initial_color.G + 0.114 * initial_color.B;
        }
    }
    return result_image;
}

Image Crop::ProcessFilter(Image &image) {
    if (params.size() < 2) {
        throw CustomExceptions("Not enough arguments or arguments are wrong");
    }
    try {
        int new_width = std::min(static_cast<size_t>(std::stoi(params[0])), image.GetWidth());
        int new_height = std::min(static_cast<size_t>(std::stoi(params[1])), image.GetHeight());
        Image result_image = Image(new_width, new_height);
        for (size_t row = 0; row < result_image.GetHeight(); ++row) {
            for (size_t column = 0; column < result_image.GetWidth(); ++column) {
                result_image.GetPixel(row, column) = image.GetPixel(row, column);
            }
        }
        return result_image;
    } catch (const std::exception &e) {
        throw CustomExceptions("Not enough arguments or arguments are wrong");
    }
}

Image Negative::ProcessFilter(Image &image) {
    Image result_image = Image(image.GetWidth(), image.GetHeight());
    for (size_t row = 0; row < result_image.GetHeight(); ++row) {
        for (size_t column = 0; column < result_image.GetWidth(); ++column) {
            result_image.GetPixel(row, column).R = 255 - image.GetPixel(row, column).R;
            result_image.GetPixel(row, column).G = 255 - image.GetPixel(row, column).G;
            result_image.GetPixel(row, column).B = 255 - image.GetPixel(row, column).B;
        }
    }
    return result_image;
}

Image EdgeDetection::ProcessFilter(
    Image &image) {  // этот через вызов ProcessFilter для default matrix(считается каждый цвет отдельно),
    // закоменченный ProcessFilter только для Edge (все цвета считаются одновременно, но кода чуть больше). Я бы выбрал
    // второй из-за быстроты
    Grayscale grayscale_filter;
    Image result_image = grayscale_filter.ProcessFilter(image);
    MatrixFilter matrix_filter = MatrixFilter(EDGE_MATRIX);
    result_image = matrix_filter.ProcessFilter(result_image);

    int w = image.GetWidth();
    int h = image.GetHeight();
    for (int row = 0; row < h; ++row) {
        for (int column = 0; column < w; ++column) {
            if (result_image.GetPixel(row, column).R > threshold) {
                result_image.GetPixel(row, column).R = 255;
                result_image.GetPixel(row, column).G = 255;
                result_image.GetPixel(row, column).B = 255;
            } else {
                result_image.GetPixel(row, column).R = 0;
                result_image.GetPixel(row, column).G = 0;
                result_image.GetPixel(row, column).B = 0;
            }
        }
    }
    return result_image;
}

// Image EdgeDetection::ProcessFilter(Image &image) {
//     Grayscale grayscale_filter;
//     Image result_image = grayscale_filter.ProcessFilter(image);
//
//
//     int w = image.GetWidth();
//     int h = image.GetHeight();
//     for (int row = 0; row < h; ++row) {
//         for (int column = 0; column < w; ++column) {
//             int new_color = 0;
//             for (auto change : NEIGHBOURS_SHIFT) {
//                 size_t new_x = std::max(0, std::min(h - 1, row + change.first));
//                 size_t new_y = std::max(0, std::min(w - 1, column + change.second));
//                 new_color += (-1) * static_cast<int>(image.GetPixel(new_x, new_y).R);
//             }
//             new_color += 4 * static_cast<int>(image.GetPixel(row, column).R);
//             if (new_color > threshold) {
//                 result_image.GetPixel(row, column).R = 255;
//                 result_image.GetPixel(row, column).G = 255;
//                 result_image.GetPixel(row, column).B = 255;
//             } else {
//                 result_image.GetPixel(row, column).R = 0;
//                 result_image.GetPixel(row, column).G = 0;
//                 result_image.GetPixel(row, column).B = 0;
//             }
//         }
//     }
//     return result_image;
// }

EdgeDetection::EdgeDetection(std::vector<std::string> params) {
    if (params.empty()) {
        throw CustomExceptions("Not enough arguments or arguments are wrong");
    }
    threshold = std::stoi(params[0]);
}

Image GaussianBlurFilter::ProcessFilter(Image &image) {
    try {
        int sigma = std::stoi(params[0]);
        FillMatrixCoefficients(sigma);
        Image result_image = image;
        auto middle_pos = gauss_size_ / 2;
        Image pre_res = result_image;
        for (size_t row = 0; row < pre_res.GetHeight(); ++row) {
            for (size_t column = 0; column < pre_res.GetWidth(); ++column) {
                double new_red = 0;
                double new_green = 0;
                double new_blue = 0;
                for (size_t i = 0; i < gauss_coefficients_.size(); ++i) {
                    size_t new_x =
                        std::min(pre_res.GetHeight() - 1, std::max(row + i - middle_pos, static_cast<size_t>(0)));
                    new_red += pre_res.GetPixel(new_x, column).R * gauss_coefficients_[i];
                    new_green += pre_res.GetPixel(new_x, column).G * gauss_coefficients_[i];
                    new_blue += pre_res.GetPixel(new_x, column).B * gauss_coefficients_[i];
                }
                new_red /= coefficients_sum_;
                new_green /= coefficients_sum_;
                new_blue /= coefficients_sum_;
                result_image.GetPixel(row, column).R = static_cast<uint8_t>(new_red);
                result_image.GetPixel(row, column).G = static_cast<uint8_t>(new_green);
                result_image.GetPixel(row, column).B = static_cast<uint8_t>(new_blue);
            }
        }
        pre_res = result_image;
        for (size_t row = 0; row < pre_res.GetHeight(); ++row) {
            for (size_t column = 0; column < pre_res.GetWidth(); ++column) {
                double new_red = 0;
                double new_green = 0;
                double new_blue = 0;
                for (size_t i = 0; i < gauss_coefficients_.size(); ++i) {
                    size_t new_y =
                        std::min(pre_res.GetWidth() - 1, std::max(column + i - middle_pos, static_cast<size_t>(0)));
                    new_red += pre_res.GetPixel(row, new_y).R * gauss_coefficients_[i];
                    new_green += pre_res.GetPixel(row, new_y).G * gauss_coefficients_[i];
                    new_blue += pre_res.GetPixel(row, new_y).B * gauss_coefficients_[i];
                }
                new_red /= coefficients_sum_;
                new_green /= coefficients_sum_;
                new_blue /= coefficients_sum_;
                result_image.GetPixel(row, column).R = static_cast<uint8_t>(new_red);
                result_image.GetPixel(row, column).G = static_cast<uint8_t>(new_green);
                result_image.GetPixel(row, column).B = static_cast<uint8_t>(new_blue);
            }
        }
        return result_image;
    } catch (const std::exception &e) {
        throw CustomExceptions("Error with reading arguments");
    }
}

void GaussianBlurFilter::FillMatrixCoefficients(int sigma) {
    gauss_size_ = 6 * sigma + 1;
    gauss_coefficients_.resize(gauss_size_);
    int middle_pos = 3 * sigma;
    for (size_t i = 0; i < gauss_size_; ++i) {
        gauss_coefficients_[i] =
            std::pow(M_E, static_cast<double>(-std::pow(i - middle_pos, 2)) / (2 * sigma * sigma)) /
            std::sqrt(2 * M_PI * sigma * sigma);
        coefficients_sum_ += gauss_coefficients_[i];
    }
}

Image TrianglesFilter::ProcessFilter(Image &image) {
    int triangle_size = std::stoi(params[0]);

    std::vector<std::vector<std::vector<Image::Pixel>>> pre_vec;
    pre_vec.reserve(image.GetHeight() / triangle_size);
    for (size_t i = 0; i < image.GetHeight() / triangle_size; ++i) {
        std::vector<std::vector<Image::Pixel>> pre_vvec;
        pre_vvec.resize((image.GetWidth() / triangle_size) * 2);
        pre_vec.push_back(pre_vvec);
    }

    for (size_t row = 0; row < (image.GetHeight() / triangle_size) * triangle_size; ++row) {
        for (size_t column = 0; column < (image.GetWidth() / triangle_size) * triangle_size; ++column) {
            if (row % triangle_size + column % triangle_size > static_cast<size_t>(triangle_size)) {  //
                pre_vec[row / triangle_size][(column / triangle_size) * 2 + 1].push_back(image.GetPixel(row, column));
            } else {
                pre_vec[row / triangle_size][(column / triangle_size) * 2].push_back(image.GetPixel(row, column));
            }
        }
    }

    std::vector<std::vector<Image::Pixel>> new_colors;
    new_colors.reserve(image.GetHeight() / triangle_size);
    for (size_t i = 0; i < pre_vec.size(); ++i) {
        std::vector<Image::Pixel> pre_col;
        pre_col.resize((image.GetWidth() / triangle_size) * 2);
        new_colors.push_back(pre_col);
        for (size_t j = 0; j < pre_col.size(); ++j) {
            new_colors[i][j] = Average(pre_vec[i][j]);
        }
    }

    Image result =
        Image((image.GetWidth() / triangle_size) * triangle_size, (image.GetHeight() / triangle_size) * triangle_size);
    for (size_t row = 0; row < result.GetHeight(); ++row) {
        for (size_t column = 0; column < result.GetWidth(); ++column) {
            if (row % triangle_size + column % triangle_size > static_cast<size_t>(triangle_size)) {
                result.GetPixel(row, column) = new_colors[row / triangle_size][(column / triangle_size) * 2 + 1];
            } else {
                result.GetPixel(row, column) = new_colors[row / triangle_size][(column / triangle_size) * 2];
            }
        }
    }
    return result;
}

Image::Pixel TrianglesFilter::Average(const std::vector<Image::Pixel> &v) {
    Image::Pixel result;
    size_t new_r = 0;  // защита от переполнения в result
    size_t new_g = 0;
    size_t new_b = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        new_r += v[i].R;
        new_g += v[i].G;
        new_b += v[i].B;
    }
    result.R = new_r / v.size();
    result.G = new_g / v.size();
    result.B = new_b / v.size();
    return result;
}