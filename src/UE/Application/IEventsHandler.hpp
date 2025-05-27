#pragma once

#include "Ports/IBtsPort.hpp"
#include "Ports/ITimerPort.hpp"
#include "Ports/IUserPort.hpp"

namespace ue
{
class IEventsHandler : public IBtsEventsHandler, public IUserEventsHandler, public ITimerEventsHandler
{
 public:
    virtual ~IEventsHandler() = default;
};

} // namespace ue
