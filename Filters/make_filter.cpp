#include "make_filter.h"

#include "stdexcept"

namespace make_filters {
    Filter* MakeGrayscaleFilter(const Token& token) {
        if (!token.params.empty()) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        return new GreyscaleFilter;
    }
    Filter* MakeNegativeFilter(const Token& token) {
        if (!token.params.empty()) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        return new NegativeFilter;
    }
    Filter* MakeCropFilter(const Token& token) {
        if (token.params.size() != 2) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        std::string height = {token.params[0].begin(), token.params[0].end()};
        std::string width = {token.params[1].begin(), token.params[1].end()};
        return new CropFilter(std::stoi(height), std::stoi(width));
    }
    Filter* MakeEdgeDetectionFilter(const Token& token) {
        if (token.params.size() != 1) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        std::string threshold = {token.params[0].begin(), token.params[0].end()};
        return new EdgeDetectionFilter(std::stof(threshold));
    }
    Filter* MakeSharpeningFilter(const Token& token) {
        if (!token.params.empty()) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        return new SharpeningFilter;
    }
    Filter* MakeGaussianBlurFilter(const Token& token) {
        if (token.params.size() != 1) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        std::string sigma = {token.params[0].begin(), token.params[0].end()};
        return new GaussianBlurFilter(std::stoi(sigma));
    }
    Filter* MakeCrystallizeFilter(const Token& token) {
        if (token.params.size() != 1) {
            throw std::invalid_argument("Invalid number of parameters");
        }
        std::string size = {token.params[0].begin(), token.params[0].end()};
        return new CrystallizeFilter(std::stoi(size));
    }
}  // namespace make_filters

void FilterFactory::AddFilterMaker(std::string_view name, FilterFac maker) {
    filters_.insert({name, maker});
}

FilterFactory::FilterFac FilterFactory::GetFilterMaker(std::string_view name) const {
    FilterMap::const_iterator it = filters_.find(name);
    if (it == filters_.end()) {
        return nullptr;
    }
    return it->second;
}

Filter* FilterFactory::CreateFilter(const Token& token) const {
    FilterFac maker = GetFilterMaker(token.name);
    if (!maker) {
        return nullptr;
    }
    return maker(token);
}
