#pragma once

#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleAccept() = 0;
    virtual void handleReject() = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;

    virtual void showCallRequest(common::PhoneNumber from) = 0;
    virtual void showCallAccepted(common::PhoneNumber from) = 0;
    virtual void showCallDropped(common::PhoneNumber from) = 0;
    virtual void showIncomingText(common::PhoneNumber from, const std::string& text) = 0;
};

}
