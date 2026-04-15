#pragma once

#include <cstdint>
#include <random>

namespace city::util {

class Random {
public:
    explicit Random(std::uint32_t seed);

    std::uint32_t nextUInt32();
    std::uint32_t nextUInt32(std::uint32_t upper_bound_exclusive);

private:
    std::mt19937 engine_;
};

} // namespace city::util
