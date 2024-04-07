#include "image.h"

#include <stdexcept>

bool Image::Read(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }
    Read(file);
    file.close();
    return true;
}

bool Image::Read(std::istream& input) {
    input.read(reinterpret_cast<char*>(&bmp_header_), sizeof(bmp_header_));
    input.read(reinterpret_cast<char*>(&dib_header_), sizeof(dib_header_));
    if (dib_header_.bmp_height < 0 || dib_header_.bmp_width < 0) {
        throw std::runtime_error("Invalid image size");
    }
    const int64_t rubbish_size = (4 - (dib_header_.bmp_width * 3) % 4) % 4;
    pixels_.SetHeight(dib_header_.bmp_height);
    pixels_.SetWidth(dib_header_.bmp_width);
    for (size_t i = 0; i < dib_header_.bmp_height; ++i) {
        for (size_t j = 0; j < dib_header_.bmp_width; ++j) {
            Color color{};
            input.read(reinterpret_cast<char*>(&color), sizeof(color));
            pixels_.PushBack(color);
        }
        input.ignore(rubbish_size);
    }
    return true;
}
