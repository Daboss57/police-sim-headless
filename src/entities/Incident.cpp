#include "entities/Incident.hpp"

#include <stdexcept>

namespace city::entities {

Incident::Incident(
    IncidentId id,
    map::NodeId location,
    IncidentSeverity severity,
    std::uint64_t reported_tick)
    : id_(id)
    , location_(location)
    , severity_(severity)
    , reported_tick_(reported_tick)
    , last_updated_tick_(reported_tick)
{
}

IncidentId Incident::id() const
{
    return id_;
}

map::NodeId Incident::location() const
{
    return location_;
}

IncidentSeverity Incident::severity() const
{
    return severity_;
}

IncidentStatus Incident::status() const
{
    return status_;
}

std::uint64_t Incident::reportedTick() const
{
    return reported_tick_;
}

std::uint64_t Incident::lastUpdatedTick() const
{
    return last_updated_tick_;
}

void Incident::markAssigned(std::uint64_t tick)
{
    if (status_ != IncidentStatus::Reported) {
        throw std::logic_error("only reported incidents can be assigned");
    }

    setStatus(IncidentStatus::Assigned, tick);
}

void Incident::resolve(std::uint64_t tick)
{
    if (status_ == IncidentStatus::Resolved || status_ == IncidentStatus::Cancelled) {
        throw std::logic_error("closed incidents cannot be resolved again");
    }

    setStatus(IncidentStatus::Resolved, tick);
}

void Incident::cancel(std::uint64_t tick)
{
    if (status_ == IncidentStatus::Resolved || status_ == IncidentStatus::Cancelled) {
        throw std::logic_error("closed incidents cannot be cancelled again");
    }

    setStatus(IncidentStatus::Cancelled, tick);
}

void Incident::setStatus(IncidentStatus status, std::uint64_t tick)
{
    if (tick < last_updated_tick_) {
        throw std::invalid_argument("incident update tick cannot move backward");
    }

    status_ = status;
    last_updated_tick_ = tick;
}

std::string_view toString(IncidentSeverity severity)
{
    switch (severity) {
    case IncidentSeverity::Low:
        return "low";
    case IncidentSeverity::Medium:
        return "medium";
    case IncidentSeverity::High:
        return "high";
    case IncidentSeverity::Critical:
        return "critical";
    }

    return "unknown";
}

std::string_view toString(IncidentStatus status)
{
    switch (status) {
    case IncidentStatus::Reported:
        return "reported";
    case IncidentStatus::Assigned:
        return "assigned";
    case IncidentStatus::Resolved:
        return "resolved";
    case IncidentStatus::Cancelled:
        return "cancelled";
    }

    return "unknown";
}

} // namespace city::entities
