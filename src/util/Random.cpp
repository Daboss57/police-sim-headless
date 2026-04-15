#include "util/Random.hpp"

#include <limits>
#include <stdexcept>

namespace city::util {

Random::Random(std::uint32_t seed)
    : engine_(seed)
{
}

std::uint32_t Random::nextUInt32()
{
    return engine_();
}

std::uint32_t Random::nextUInt32(std::uint32_t upper_bound_exclusive)
{
    if (upper_bound_exclusive == 0) {
        throw std::invalid_argument("upper_bound_exclusive must be greater than zero");
    }

    const std::uint64_t range =
        static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()) + 1ULL;
    const std::uint64_t bucket_size = range / upper_bound_exclusive;
    const std::uint64_t limit = bucket_size * upper_bound_exclusive;

    std::uint32_t value = nextUInt32();
    while (value >= limit) {
        value = nextUInt32();
    }

    return static_cast<std::uint32_t>(value / bucket_size);
}

} // namespace city::util
