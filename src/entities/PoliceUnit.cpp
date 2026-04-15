#include "entities/PoliceUnit.hpp"

#include <stdexcept>
#include <utility>

namespace city::entities {

PoliceUnit::PoliceUnit(PoliceUnitId id, map::NodeId start_node)
    : id_(id)
    , current_node_(start_node)
{
}

PoliceUnitId PoliceUnit::id() const
{
    return id_;
}

map::NodeId PoliceUnit::currentNode() const
{
    return current_node_;
}

PoliceUnitStatus PoliceUnit::status() const
{
    return status_;
}

std::optional<IncidentId> PoliceUnit::assignedIncident() const
{
    return assigned_incident_;
}

const std::vector<map::NodeId>& PoliceUnit::route() const
{
    return route_;
}

std::size_t PoliceUnit::routeIndex() const
{
    return route_index_;
}

void PoliceUnit::assignToIncident(IncidentId incident_id, std::vector<map::NodeId> route)
{
    if (status_ != PoliceUnitStatus::Available) {
        throw std::logic_error("only available units can be assigned");
    }

    if (route.empty() || route.front() != current_node_) {
        throw std::invalid_argument("assigned route must start at the unit's current node");
    }

    assigned_incident_ = incident_id;
    route_ = std::move(route);
    route_index_ = 0;
    status_ = route_.size() == 1 ? PoliceUnitStatus::OnScene : PoliceUnitStatus::EnRoute;
}

bool PoliceUnit::advanceOneTick()
{
    if (status_ != PoliceUnitStatus::EnRoute) {
        return false;
    }

    ++route_index_;
    current_node_ = route_.at(route_index_);

    if (route_index_ + 1 == route_.size()) {
        status_ = PoliceUnitStatus::OnScene;
        return true;
    }

    return false;
}

void PoliceUnit::clearAssignment()
{
    if (status_ == PoliceUnitStatus::OutOfService) {
        throw std::logic_error("out-of-service units cannot clear assignments");
    }

    assigned_incident_.reset();
    route_.clear();
    route_index_ = 0;
    status_ = PoliceUnitStatus::Available;
}

void PoliceUnit::setOutOfService()
{
    assigned_incident_.reset();
    route_.clear();
    route_index_ = 0;
    status_ = PoliceUnitStatus::OutOfService;
}

void PoliceUnit::returnToService(map::NodeId node)
{
    current_node_ = node;
    assigned_incident_.reset();
    route_.clear();
    route_index_ = 0;
    status_ = PoliceUnitStatus::Available;
}

std::string_view toString(PoliceUnitStatus status)
{
    switch (status) {
    case PoliceUnitStatus::Available:
        return "available";
    case PoliceUnitStatus::EnRoute:
        return "en route";
    case PoliceUnitStatus::OnScene:
        return "on scene";
    case PoliceUnitStatus::OutOfService:
        return "out of service";
    }

    return "unknown";
}

} // namespace city::entities
