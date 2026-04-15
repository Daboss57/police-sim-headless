#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace city::map {

using NodeId = std::uint32_t;
using TravelTimeTicks = std::uint32_t;

struct RoadEdge {
    NodeId to;
    TravelTimeTicks travel_time;
};

class RoadGraph {
public:
    bool addNode(NodeId id);
    void addDirectedRoad(NodeId from, NodeId to, TravelTimeTicks travel_time);
    void addBidirectionalRoad(NodeId a, NodeId b, TravelTimeTicks travel_time);

    bool containsNode(NodeId id) const;
    const std::vector<RoadEdge>& outgoingRoads(NodeId id) const;
    std::vector<NodeId> nodes() const;

    std::size_t nodeCount() const;
    std::size_t roadCount() const;

private:
    std::unordered_map<NodeId, std::vector<RoadEdge>> adjacency_;
    std::size_t road_count_ = 0;
};

} // namespace city::map
