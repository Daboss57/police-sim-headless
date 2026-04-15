#include "map/RoadGraph.hpp"
#include "map/ShortestPath.hpp"
#include "util/Random.hpp"

#include <iostream>

namespace {

city::map::RoadGraph createSampleRoadGraph()
{
    city::map::RoadGraph graph;

    for (city::map::NodeId id = 1; id <= 5; ++id) {
        graph.addNode(id);
    }

    graph.addBidirectionalRoad(1, 2, 4);
    graph.addBidirectionalRoad(2, 3, 6);
    graph.addBidirectionalRoad(1, 4, 12);
    graph.addBidirectionalRoad(4, 5, 3);
    graph.addBidirectionalRoad(5, 3, 4);

    return graph;
}

void printPath(const city::map::ShortestPathResult& path)
{
    if (!path.reachable) {
        std::cout << "sample path: unreachable\n";
        return;
    }

    std::cout << "sample path:";
    for (city::map::NodeId node : path.nodes) {
        std::cout << ' ' << node;
    }
    std::cout << " (" << path.travel_time << " ticks)\n";
}

} // namespace

int main()
{
    city::util::Random random(42);
    const city::map::RoadGraph graph = createSampleRoadGraph();
    const city::map::ShortestPathResult path = city::map::findShortestPath(graph, 1, 3);

    std::cout << "city-response-sim starting\n";
    std::cout << "road graph: " << graph.nodeCount() << " nodes, " << graph.roadCount()
              << " directed roads\n";
    printPath(path);
    std::cout << "seeded sample: " << random.nextUInt32(100) << '\n';

    return 0;
}
