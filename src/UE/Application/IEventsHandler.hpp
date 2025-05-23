#pragma once

#include "Ports/ITimerPort.hpp"
#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"


namespace ue
{
  namespace details
  {
    enum class GuiViewState : uint8_t
    {
      UNKNOWN = 0,
      MAIN_MENU = 1,
      SMS_MENU = 2,
      SMS_LIST = 3,
      SMS_VIEW = 4
    };
  }

class IEventsHandler : public IBtsEventsHandler,
                       public IUserEventsHandler,
                       public ITimerEventsHandler
{
  public:
    virtual ~IEventsHandler() = default;
    virtual void handleUiAction(std::optional<std::size_t> selectedIndex) = 0;
    virtual void handleUiBack() = 0;
};

}
