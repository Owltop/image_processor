#include "filter_factory.h"

FilterFactory::FilterFactory(Image& image, std::vector<Filter>& filters) {
    try {
        Image result_image = image;
        for (const auto& filter : filters) {
            filters_pipeline.push_back(filters_map_[filter.filter_name_](result_image, filter.params_));
        }
    } catch (const CustomExceptions& e) {
        std::cout << e.message << std::endl;
        throw CustomExceptions(
            "Error with creations filters");  // Как по мне, такое сообщение лучше, чем чёткое указание, что не так.
                                              // Так как возможно ошибка произошла по другой причине и пользователя это
                                              // собьёт с толку.
    } catch (const std::exception& e) {
        throw CustomExceptions("Error with creations filters");
    }
}
std::vector<std::unique_ptr<BaseFilter>>& FilterFactory::GetPipeline() {
    return filters_pipeline;
}

std::unique_ptr<Crop> CreateCropFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<Crop>(params);
}

std::unique_ptr<Grayscale> CreateGrayscaleFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<Grayscale>(params);
}

std::unique_ptr<Sharpening> CreateSharpeningFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<Sharpening>(params);
}

std::unique_ptr<Negative> CreateNegativeFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<Negative>(params);
}

std::unique_ptr<EdgeDetection> CreateEdgeDetectionFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<EdgeDetection>(params);
}

std::unique_ptr<GaussianBlurFilter> CreateGaussianBlurFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<GaussianBlurFilter>(params);
}

std::unique_ptr<TrianglesFilter> CreateTrianglesFilter(Image& image, std::vector<std::string> params) {
    return std::make_unique<TrianglesFilter>(params);
}