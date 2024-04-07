#include "image.h"

#include <stdexcept>

static const uint32_t RUBBISH = 0x55AA55AA;

bool Image::Write(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }
    Write(file);
    file.close();
    return true;
}

bool Image::Write(std::ostream& output) {
    output.write(reinterpret_cast<const char*>(&bmp_header_), sizeof(bmp_header_));
    output.write(reinterpret_cast<const char*>(&dib_header_), sizeof(dib_header_));
    const int64_t rubbish_size = (4 - (dib_header_.bmp_width * 3) % 4) % 4;
    for (size_t i = 0; i < dib_header_.bmp_height; ++i) {
        for (size_t j = 0; j < dib_header_.bmp_width; ++j) {
            const Color& color = pixels_.GetColor(i, j);
            output.write(reinterpret_cast<const char*>(&color), sizeof(pixels_.GetColor(i, j)));
        }
        output.write(reinterpret_cast<const char*>(&RUBBISH), rubbish_size);
    }
    return true;
}
