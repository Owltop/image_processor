#pragma once

#include "parser.h"
#include "filters.h"

#include <map>
#include <functional>
#include <memory>

std::unique_ptr<Crop> CreateCropFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<Grayscale> CreateGrayscaleFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<Sharpening> CreateSharpeningFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<Negative> CreateNegativeFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<EdgeDetection> CreateEdgeDetectionFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<GaussianBlurFilter> CreateGaussianBlurFilter(Image& image, std::vector<std::string> params = {});
std::unique_ptr<TrianglesFilter> CreateTrianglesFilter(Image& image, std::vector<std::string> params = {});

class FilterFactory {
public:
    FilterFactory(Image& image, std::vector<Filter>& filters);
    std::vector<std::unique_ptr<BaseFilter>>& GetPipeline();

protected:
    std::map<std::string, std::function<std::unique_ptr<BaseFilter>(Image&, std::vector<std::string>)>> filters_map_ = {
        {"-crop", CreateCropFilter},         {"-gs", CreateGrayscaleFilter},       {"-sharp", CreateSharpeningFilter},
        {"-neg", CreateNegativeFilter},      {"-edge", CreateEdgeDetectionFilter}, {"-blur", CreateGaussianBlurFilter},
        {"-triangle", CreateTrianglesFilter}};
    std::vector<std::unique_ptr<BaseFilter>> filters_pipeline;
};