cmake_minimum_required(VERSION 3.20)

project(city-response-sim
    VERSION 0.1.0
    LANGUAGES CXX
)

add_executable(city_response_sim
    src/main.cpp
)

target_compile_features(city_response_sim PRIVATE cxx_std_20)
set_target_properties(city_response_sim PROPERTIES
    CXX_EXTENSIONS OFF
)
