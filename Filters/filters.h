#pragma once

#include "../Image/image.h"

#include <memory>
#include <cmath>
#include <algorithm>
#include <vector>
#include <random>
#include <numbers>

const int MINUS_ONE = -1;
const int ZERO = 0;
const int FOUR = 4;
const int FIVE = 5;

class Filter {
public:
    virtual void Apply(Image& img) = 0;
    virtual ~Filter() = default;
};

class GreyscaleFilter : public Filter {
public:
    void Apply(Image& img) override;
};

class NegativeFilter : public Filter {
public:
    void Apply(Image& img) override;
};

class CropFilter : public Filter {
private:
    size_t height_;
    size_t width_;

public:
    CropFilter(size_t height, size_t width) : height_(height), width_(width) {
    }
    void Apply(Image& img) override;
};

class EdgeDetectionFilter : public Filter {
private:
    float threshold_;
    std::vector<std::vector<int>> edge_matrix_ = {
            {ZERO, MINUS_ONE, ZERO}, {MINUS_ONE, FOUR, MINUS_ONE}, {ZERO, MINUS_ONE, ZERO}};

public:
    explicit EdgeDetectionFilter(float threshold) : threshold_(threshold) {
    }
    void Apply(Image& img) override;
};

class SharpeningFilter : public Filter {
private:
    std::vector<std::vector<int>> sharpening_matrix_ = {
            {ZERO, MINUS_ONE, ZERO}, {MINUS_ONE, FIVE, MINUS_ONE}, {ZERO, MINUS_ONE, ZERO}};

public:
    void Apply(Image& img) override;
};

class GaussianBlurFilter : public Filter {
private:
    int32_t sigma_;
    std::vector<std::vector<float>> matrix_;

public:
    explicit GaussianBlurFilter(int32_t sigma) : sigma_(sigma) {
    }
    float GetGaussianValue(int32_t sigma, int32_t x, int32_t y);
    void Apply(Image& img) override;
};

class CrystallizeFilter : public Filter {
private:
    size_t cells_cnt_;

public:
    explicit CrystallizeFilter(size_t cells_cnt) : cells_cnt_(cells_cnt) {
    }
    void Apply(Image& img) override;
};
