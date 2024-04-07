#pragma once

#include "../Parser/parse.h"
#include "filters.h"

#include <map>

namespace make_filters {
    Filter* MakeGrayscaleFilter(const Token& token);
    Filter* MakeNegativeFilter(const Token& token);
    Filter* MakeCropFilter(const Token& token);
    Filter* MakeEdgeDetectionFilter(const Token& token);
    Filter* MakeSharpeningFilter(const Token& token);
    Filter* MakeGaussianBlurFilter(const Token& token);
    Filter* MakeCrystallizeFilter(const Token& token);
}  // namespace make_filters

class FilterFactory {
public:
    using FilterFac = Filter* (*)(const Token&);
    using FilterMap = std::map<std::string_view, FilterFac>;

    FilterFactory()
            : filters_({{"-gs", &make_filters::MakeGrayscaleFilter},
                        {"-neg", &make_filters::MakeNegativeFilter},
                        {"-crop", &make_filters::MakeCropFilter},
                        {"-edge", &make_filters::MakeEdgeDetectionFilter},
                        {"-sharp", &make_filters::MakeSharpeningFilter},
                        {"-blur", &make_filters::MakeGaussianBlurFilter},
                        {"-crystal", &make_filters::MakeCrystallizeFilter}}){};

    void AddFilterMaker(std::string_view name, FilterFac maker);
    FilterFac GetFilterMaker(std::string_view name) const;
    Filter* CreateFilter(const Token& token) const;

protected:
    FilterMap filters_;
};
