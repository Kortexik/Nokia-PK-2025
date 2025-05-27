#pragma once

#include "Messages/PhoneNumber.hpp"
#include <functional>
#include <string>

namespace ue
{

using common::PhoneNumber;

class IUeGui
{
 public:
    class IListViewMode;
    class ISmsComposeMode;
    class IDialMode;
    class ICallMode;
    class ITextMode;

    using AcceptClose = bool;
    using CloseGuard = std::function<AcceptClose()>;
    using Callback = std::function<void()>;

    virtual ~IUeGui() = default;

    virtual void setCloseGuard(CloseGuard closeGuard) = 0;
    virtual void setAcceptCallback(Callback) = 0;
    virtual void setRejectCallback(Callback) = 0;

    virtual void setTitle(const std::string &title) = 0;
    virtual void showConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showNotConnected() = 0;
    virtual void showNewSms(bool present) = 0;
    virtual void showPeerUserNotAvailable(PhoneNumber) = 0;

    virtual IListViewMode &setListViewMode() = 0;
    virtual ISmsComposeMode &setSmsComposeMode() = 0;
    virtual IDialMode &setDialMode() = 0;
    virtual ICallMode &setCallMode() = 0;
    virtual ITextMode &setAlertMode() = 0;
    virtual ITextMode &setViewTextMode() = 0;
};

} // namespace ue
