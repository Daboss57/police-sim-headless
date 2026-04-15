#include "entities/Incident.hpp"
#include "entities/PoliceUnit.hpp"
#include "map/RoadGraph.hpp"
#include "map/ShortestPath.hpp"
#include "util/Random.hpp"

#include <cstdint>
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

void printUnitState(const city::entities::PoliceUnit& unit)
{
    std::cout << "unit " << unit.id() << ": " << city::entities::toString(unit.status())
              << " at node " << unit.currentNode() << '\n';
}

} // namespace

int main()
{
    city::util::Random random(42);
    const city::map::RoadGraph graph = createSampleRoadGraph();
    const city::map::ShortestPathResult path = city::map::findShortestPath(graph, 1, 3);
    city::entities::Incident incident(
        1001,
        3,
        city::entities::IncidentSeverity::High,
        0);
    city::entities::PoliceUnit unit(7, 1);
    std::uint64_t current_tick = 0;

    std::cout << "city-response-sim starting\n";
    std::cout << "road graph: " << graph.nodeCount() << " nodes, " << graph.roadCount()
              << " directed roads\n";
    printPath(path);

    std::cout << "incident " << incident.id() << ": "
              << city::entities::toString(incident.severity()) << " severity at node "
              << incident.location() << '\n';

    if (path.reachable) {
        ++current_tick;
        incident.markAssigned(current_tick);
        unit.assignToIncident(incident.id(), path.nodes);
    }

    printUnitState(unit);
    while (unit.status() == city::entities::PoliceUnitStatus::EnRoute) {
        ++current_tick;
        const bool arrived = unit.advanceOneTick();
        printUnitState(unit);
        if (arrived) {
            incident.resolve(current_tick);
        }
    }

    std::cout << "incident " << incident.id() << ": "
              << city::entities::toString(incident.status()) << '\n';
    std::cout << "seeded sample: " << random.nextUInt32(100) << '\n';

    return 0;
}
