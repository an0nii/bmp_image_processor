#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <tuple>
#include <fstream>
#include <string>

class Image {
public:
    struct BMPHeader {
        uint16_t signature;
        uint32_t file_size;
        uint32_t reserved;
        uint32_t data_offset;
    } __attribute__((packed));

    struct DIBHeader {
        uint32_t header_size;
        uint32_t bmp_width;
        uint32_t bmp_height;
        uint16_t color_planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t x_pixels_per_meter;
        uint32_t y_pixels_per_meter;
        uint32_t colors_in_color_table;
        uint32_t important_color_count;
    } __attribute__((packed));

    struct Color {
        uint8_t blue = 0;
        uint8_t green = 0;
        uint8_t red = 0;

        bool operator==(const Color& other) const {
            return std::tie(blue, green, red) == std::tie(other.blue, other.green, other.red);
        }

        bool operator!=(const Color& other) const {
            return !(*this == other);
        }
    };

    class Pixels {
    protected:
        std::vector<Color> data_;
        size_t width_;
        size_t height_;

    public:
        Pixels(){};

        void SetHeight(size_t height) {
            height_ = height;
        }

        void SetWidth(size_t width) {
            width_ = width;
        }

        void PushBack(const Color& color) {
            data_.push_back(color);
        }

        const Color& GetColor(size_t row, size_t column) const {
            return data_[row * width_ + column];
        }

        Color& GetColor(size_t row, size_t column) {
            return data_[row * width_ + column];
        }

        size_t GetHeight() const {
            return height_;
        }

        size_t GetWidth() const {
            return width_;
        }

        bool IsEmpty() const {
            return data_.empty();
        }
    };

    void Copy(const Pixels& other) {
        pixels_ = other;
    }

protected:
    BMPHeader bmp_header_;
    DIBHeader dib_header_;
    Pixels pixels_;

public:
    void SetFileHeight(size_t height) {
        dib_header_.bmp_height = height;
    }

    void SetFileWidth(size_t width) {
        dib_header_.bmp_width = width;
    }

    const Color& GetColor(size_t row, size_t column) const {
        return pixels_.GetColor(row, column);
    }

    Color& GetColor(size_t row, size_t column) {
        return pixels_.GetColor(row, column);
    }

    size_t GetHeight() const {
        return pixels_.GetHeight();
    }

    size_t GetWidth() const {
        return pixels_.GetWidth();
    }

    bool IsEmpty() const {
        return pixels_.IsEmpty();
    }

    // Reader and Writer
    bool Read(std::istream& input);

    bool Read(const char* file_name);

    bool Write(std::ostream& output);

    bool Write(const char* file_name);
};
