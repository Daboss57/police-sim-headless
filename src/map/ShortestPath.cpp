#include "map/ShortestPath.hpp"

#include <algorithm>
#include <limits>
#include <queue>
#include <unordered_map>

namespace city::map {

namespace {

struct QueueEntry {
    NodeId node;
    std::uint64_t travel_time;
};

struct QueueEntryGreater {
    bool operator()(const QueueEntry& left, const QueueEntry& right) const
    {
        if (left.travel_time != right.travel_time) {
            return left.travel_time > right.travel_time;
        }

        return left.node > right.node;
    }
};

std::vector<NodeId> buildPath(
    const std::unordered_map<NodeId, NodeId>& previous,
    NodeId start,
    NodeId goal)
{
    std::vector<NodeId> path;

    for (NodeId current = goal;; current = previous.at(current)) {
        path.push_back(current);
        if (current == start) {
            break;
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

} // namespace

ShortestPathResult findShortestPath(const RoadGraph& graph, NodeId start, NodeId goal)
{
    if (!graph.containsNode(start) || !graph.containsNode(goal)) {
        return {};
    }

    if (start == goal) {
        return ShortestPathResult{true, 0, {start}};
    }

    std::priority_queue<QueueEntry, std::vector<QueueEntry>, QueueEntryGreater> frontier;
    std::unordered_map<NodeId, std::uint64_t> best_time;
    std::unordered_map<NodeId, NodeId> previous;

    frontier.push(QueueEntry{start, 0});
    best_time[start] = 0;

    while (!frontier.empty()) {
        const QueueEntry current = frontier.top();
        frontier.pop();

        if (current.travel_time != best_time.at(current.node)) {
            continue;
        }

        if (current.node == goal) {
            return ShortestPathResult{
                true,
                current.travel_time,
                buildPath(previous, start, goal),
            };
        }

        for (const RoadEdge& road : graph.outgoingRoads(current.node)) {
            const std::uint64_t max_time = std::numeric_limits<std::uint64_t>::max();
            if (current.travel_time > max_time - road.travel_time) {
                continue;
            }

            const std::uint64_t candidate_time = current.travel_time + road.travel_time;
            const auto known = best_time.find(road.to);
            if (known != best_time.end() && known->second <= candidate_time) {
                continue;
            }

            best_time[road.to] = candidate_time;
            previous[road.to] = current.node;
            frontier.push(QueueEntry{road.to, candidate_time});
        }
    }

    return {};
}

} // namespace city::map
