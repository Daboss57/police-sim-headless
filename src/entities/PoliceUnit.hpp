#pragma once

#include "entities/Incident.hpp"
#include "map/RoadGraph.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace city::entities {

using PoliceUnitId = std::uint32_t;

enum class PoliceUnitStatus {
    Available,
    EnRoute,
    OnScene,
    OutOfService,
};

class PoliceUnit {
public:
    PoliceUnit(PoliceUnitId id, map::NodeId start_node);

    PoliceUnitId id() const;
    map::NodeId currentNode() const;
    PoliceUnitStatus status() const;
    std::optional<IncidentId> assignedIncident() const;
    const std::vector<map::NodeId>& route() const;
    std::size_t routeIndex() const;

    void assignToIncident(IncidentId incident_id, std::vector<map::NodeId> route);
    bool advanceOneTick();
    void clearAssignment();
    void setOutOfService();
    void returnToService(map::NodeId node);

private:
    PoliceUnitId id_;
    map::NodeId current_node_;
    PoliceUnitStatus status_ = PoliceUnitStatus::Available;
    std::optional<IncidentId> assigned_incident_;
    std::vector<map::NodeId> route_;
    std::size_t route_index_ = 0;
};

std::string_view toString(PoliceUnitStatus status);

} // namespace city::entities
