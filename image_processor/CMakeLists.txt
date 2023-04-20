cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(image_processor
        image_processor.cpp
        bmp.cpp bmp.h
        image.h image.cpp
        parser.h parser.cpp filters.h filters.cpp filter_factory.h filter_factory.cpp struct_filter.h test.cpp)
#add_catch(test_project test.cpp
#        bmp.cpp filter_factory.cpp filters.cpp image.cpp parser.cpp)
