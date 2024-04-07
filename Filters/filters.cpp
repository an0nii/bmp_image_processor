#include "filters.h"

const int MAX_COLOR_VALUE = 255;
const float RED_COEFFICIENT = 0.299;
const float GREEN_COEFFICIENT = 0.587;
const float BLUE_COEFFICIENT = 0.114;

namespace color_helpers {
    void SetColor(Image::Color& color, float red = 0, float green = 0, float blue = 0) {
        color.red = std::min(MAX_COLOR_VALUE, std::max(0, static_cast<int32_t>(red)));
        color.green = std::min(MAX_COLOR_VALUE, std::max(0, static_cast<int32_t>(green)));
        color.blue = std::min(MAX_COLOR_VALUE, std::max(0, static_cast<int32_t>(blue)));
    }

// Function of applying the matrix to the image
    template <typename T>
    void ApplyMatrix(std::vector<std::vector<T>>& matrix, Image& img) {
        struct Pixel {
            T red = 0;
            T green = 0;
            T blue = 0;
        };
        Image::Pixels new_pixels;
        new_pixels.SetHeight(img.GetHeight());
        new_pixels.SetWidth(img.GetWidth());
        int32_t edge_value_x = matrix.size() / 2;
        int32_t edge_value_y = matrix.front().size() / 2;
        for (int32_t row = 0; row < img.GetHeight(); ++row) {
            for (int32_t column = 0; column < img.GetWidth(); ++column) {
                Image::Color color{};
                Pixel cur_colour;
                for (int32_t dx = -static_cast<int32_t>(edge_value_x); dx <= static_cast<int32_t>(edge_value_x); ++dx) {
                    for (int32_t dy = -static_cast<int32_t>(edge_value_y); dy <= static_cast<int32_t>(edge_value_y); ++dy) {
                        int32_t x = row + dx;
                        int32_t y = column + dy;
                        x = std::min(std::max(x, 0), static_cast<int32_t>(img.GetHeight()) - 1);
                        y = std::min(std::max(y, 0), static_cast<int32_t>(img.GetWidth()) - 1);
                        cur_colour.red += img.GetColor(x, y).red * matrix[dx + edge_value_x][dy + edge_value_y];
                        cur_colour.green += img.GetColor(x, y).green * matrix[dx + edge_value_x][dy + edge_value_y];
                        cur_colour.blue += img.GetColor(x, y).blue * matrix[dx + edge_value_x][dy + edge_value_y];
                    }
                }
                SetColor(color, cur_colour.red, cur_colour.green, cur_colour.blue);
                new_pixels.PushBack(color);
            }
        }
        img.Copy(new_pixels);
    }
}  // namespace color_helpers

void GreyscaleFilter::Apply(Image& img) {
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            uint8_t new_color = static_cast<uint8_t>(RED_COEFFICIENT * static_cast<float>(img.GetColor(i, j).red) +
                                                     GREEN_COEFFICIENT * static_cast<float>(img.GetColor(i, j).green) +
                                                     BLUE_COEFFICIENT * static_cast<float>(img.GetColor(i, j).blue));
            color_helpers::SetColor(img.GetColor(i, j), new_color, new_color, new_color);
        }
    }
}

void NegativeFilter::Apply(Image& img) {
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            color_helpers::SetColor(img.GetColor(i, j), static_cast<float>(MAX_COLOR_VALUE - img.GetColor(i, j).red),
                                    static_cast<float>(MAX_COLOR_VALUE - img.GetColor(i, j).green),
                                    static_cast<float>(MAX_COLOR_VALUE - img.GetColor(i, j).blue));
        }
    }
}

void CropFilter::Apply(Image& img) {
    size_t new_height = std::min(height_, img.GetHeight());
    size_t new_width = std::min(width_, img.GetWidth());
    img.SetFileHeight(new_height);
    img.SetFileWidth(new_width);
    Image::Pixels new_image;
    new_image.SetHeight(new_height);
    new_image.SetWidth(new_width);
    for (size_t i = img.GetHeight() - new_height; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < new_width; ++j) {
            new_image.PushBack(img.GetColor(i, j));
        }
    }
    img.Copy(new_image);
}

void SharpeningFilter::Apply(Image& img) {
    color_helpers::ApplyMatrix(sharpening_matrix_, img);
}

void EdgeDetectionFilter::Apply(Image& img) {
    GreyscaleFilter grayscale;
    grayscale.Apply(img);
    Image::Pixels new_pixels;
    new_pixels.SetHeight(img.GetHeight());
    new_pixels.SetWidth(img.GetWidth());
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            int color_sum = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    size_t x = std::min(std::max(static_cast<int>(i) + dx, 0), static_cast<int>(img.GetHeight() - 1));
                    size_t y = std::min(std::max(static_cast<int>(j) + dy, 0), static_cast<int>(img.GetWidth() - 1));
                    color_sum += img.GetColor(x, y).red * edge_matrix_[dx + 1][dy + 1];
                }
            }
            Image::Color new_color;
            if (static_cast<float>(color_sum) > threshold_ * MAX_COLOR_VALUE) {
                new_color.red = MAX_COLOR_VALUE;
                new_color.green = MAX_COLOR_VALUE;
                new_color.blue = MAX_COLOR_VALUE;
            } else {
                new_color.red = ZERO;
                new_color.green = ZERO;
                new_color.blue = ZERO;
            }
            new_pixels.PushBack(new_color);
        }
    }
    img.Copy(new_pixels);
}

float GaussianBlurFilter::GetGaussianValue(int32_t sigma, int32_t x, int32_t y) {
    return static_cast<float>(pow(M_E, -(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma));
}

void GaussianBlurFilter::Apply(Image& img) {
    std::vector<std::vector<float>> matrix(sigma_ * 2 + 1, std::vector<float>(sigma_ * 2 + 1));
    float sum = 0;
    for (int i = -sigma_; i <= sigma_; ++i) {
        for (int j = -sigma_; j <= sigma_; ++j) {
            matrix[i + sigma_][j + sigma_] = GetGaussianValue(sigma_, i, j);
            sum += matrix[i + sigma_][j + sigma_];
        }
    }
    for (int i = 0; i < sigma_ * 2 + 1; ++i) {
        for (int j = 0; j < sigma_ * 2 + 1; ++j) {
            matrix[i][j] /= sum;
        }
    }
    color_helpers::ApplyMatrix(matrix, img);
}

// Custom filter
void CrystallizeFilter::Apply(Image& img) {
    std::vector<std::pair<size_t, size_t>> points(cells_cnt_);
    for (size_t i = 0; i < cells_cnt_; ++i) {
        points[i].first = static_cast<uint64_t>(rand()) * (img.GetHeight() - 0) / RAND_MAX + 0;
        points[i].second = static_cast<uint64_t>(rand()) * (img.GetWidth() - 0) / RAND_MAX + 0;
    }
    std::vector<Image::Color> colors(cells_cnt_);
    for (size_t i = 0; i < cells_cnt_; i++) {
        colors[i] = img.GetColor(points[i].second, points[i].first);
    }
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            auto d_mn = std::numeric_limits<int64_t>::max();
            size_t pos = 0;
            for (size_t r = 0; r < cells_cnt_; ++r) {
                auto d = (static_cast<int64_t>(i) - static_cast<int64_t>(points[r].first)) *
                         (static_cast<int64_t>(i) - static_cast<int64_t>(points[r].first)) +
                         (static_cast<int64_t>(j) - static_cast<int64_t>(points[r].second)) *
                         (static_cast<int64_t>(j) - static_cast<int64_t>(points[r].second));
                if (d < d_mn) {
                    d_mn = d;
                    pos = r;
                }
            }
            color_helpers::SetColor(img.GetColor(i, j), colors[pos].red, colors[pos].green, colors[pos].blue);
        }
    }
}
