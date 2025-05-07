#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnected() = 0;

    virtual void handleCallRequest(common::PhoneNumber from) = 0;
    virtual void handleCallAccepted(common:: PhoneNumber from) = 0;
    virtual void handleCallDropped(common:: PhoneNumber from) = 0;
    virtual void handleCallTalk(common:: PhoneNumber from, const std::string& text) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendCallAccepted(common::PhoneNumber to) = 0;
    virtual void sendCallDrop(common::PhoneNumber to) = 0;
    virtual void sendCallRequest(common::PhoneNumber to) = 0;
};

}
