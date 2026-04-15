#include "util/Random.hpp"

#include <iostream>

int main()
{
    city::util::Random random(42);

    std::cout << "city-response-sim starting\n";
    std::cout << "seeded sample: " << random.nextUInt32(100) << '\n';

    return 0;
}
