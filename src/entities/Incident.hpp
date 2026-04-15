#pragma once

#include "map/RoadGraph.hpp"

#include <cstdint>
#include <string_view>

namespace city::entities {

using IncidentId = std::uint32_t;

enum class IncidentSeverity {
    Low,
    Medium,
    High,
    Critical,
};

enum class IncidentStatus {
    Reported,
    Assigned,
    Resolved,
    Cancelled,
};

class Incident {
public:
    Incident(
        IncidentId id,
        map::NodeId location,
        IncidentSeverity severity,
        std::uint64_t reported_tick);

    IncidentId id() const;
    map::NodeId location() const;
    IncidentSeverity severity() const;
    IncidentStatus status() const;
    std::uint64_t reportedTick() const;
    std::uint64_t lastUpdatedTick() const;

    void markAssigned(std::uint64_t tick);
    void resolve(std::uint64_t tick);
    void cancel(std::uint64_t tick);

private:
    void setStatus(IncidentStatus status, std::uint64_t tick);

    IncidentId id_;
    map::NodeId location_;
    IncidentSeverity severity_;
    IncidentStatus status_ = IncidentStatus::Reported;
    std::uint64_t reported_tick_;
    std::uint64_t last_updated_tick_;
};

std::string_view toString(IncidentSeverity severity);
std::string_view toString(IncidentStatus status);

} // namespace city::entities
