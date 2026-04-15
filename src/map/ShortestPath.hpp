#pragma once

#include "map/RoadGraph.hpp"

#include <cstdint>
#include <vector>

namespace city::map {

struct ShortestPathResult {
    bool reachable = false;
    std::uint64_t travel_time = 0;
    std::vector<NodeId> nodes;
};

ShortestPathResult findShortestPath(const RoadGraph& graph, NodeId start, NodeId goal);

} // namespace city::map
