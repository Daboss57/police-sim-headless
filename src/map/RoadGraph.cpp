#include "map/RoadGraph.hpp"

#include <algorithm>
#include <stdexcept>

namespace city::map {

bool RoadGraph::addNode(NodeId id)
{
    return adjacency_.try_emplace(id).second;
}

void RoadGraph::addDirectedRoad(NodeId from, NodeId to, TravelTimeTicks travel_time)
{
    if (travel_time == 0) {
        throw std::invalid_argument("road travel time must be greater than zero");
    }

    if (!containsNode(from) || !containsNode(to)) {
        throw std::invalid_argument("road endpoints must exist before adding a road");
    }

    adjacency_.at(from).push_back(RoadEdge{to, travel_time});
    ++road_count_;
}

void RoadGraph::addBidirectionalRoad(NodeId a, NodeId b, TravelTimeTicks travel_time)
{
    addDirectedRoad(a, b, travel_time);
    addDirectedRoad(b, a, travel_time);
}

bool RoadGraph::containsNode(NodeId id) const
{
    return adjacency_.find(id) != adjacency_.end();
}

const std::vector<RoadEdge>& RoadGraph::outgoingRoads(NodeId id) const
{
    static const std::vector<RoadEdge> empty_roads;

    const auto it = adjacency_.find(id);
    if (it == adjacency_.end()) {
        return empty_roads;
    }

    return it->second;
}

std::vector<NodeId> RoadGraph::nodes() const
{
    std::vector<NodeId> result;
    result.reserve(adjacency_.size());

    for (const auto& entry : adjacency_) {
        result.push_back(entry.first);
    }

    std::sort(result.begin(), result.end());
    return result;
}

std::size_t RoadGraph::nodeCount() const
{
    return adjacency_.size();
}

std::size_t RoadGraph::roadCount() const
{
    return road_count_;
}

} // namespace city::map
