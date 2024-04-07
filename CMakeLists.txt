cmake_minimum_required(VERSION 3.27)
project(bmp_image_processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(bmp_image_processor
        main.cpp
        Image/image.h
        Image/read.cpp
        Image/write.cpp
        Parser/parse.h
        Parser/parse.cpp
        Filters/filters.h
        Filters/make_filter.h
        Filters/filters.cpp
        Filters/make_filter.cpp
)
