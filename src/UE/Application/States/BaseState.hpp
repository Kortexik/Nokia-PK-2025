#pragma once

#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
 public:
    BaseState(Context &context, const std::string &name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;

    // IEventsHandler interface

    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;
    void handleSmsReceived(common::PhoneNumber fromNumber, const std::string &message) override;

    virtual void handleCallRequest(common::PhoneNumber from) override
    {
        logger.logDebug("Dropping a call in states that is not connected state");
        context.bts.sendCallDrop(from);
    }

    virtual void handleCallAccepted(common::PhoneNumber)
    {
    }
    virtual void handleCallDropped(common::PhoneNumber)
    {
    }

    void handleSendCallDropped(common::PhoneNumber from) override;
    void handleReceivedCallTalk(const std::string &text) override;
    void handleSendCallTalk(common::PhoneNumber to, const std::string &msg) override;

    virtual void handleMenuSelection(unsigned int index) override;

 protected:
    Context &context;
    common::PrefixedLogger logger;
};

} // namespace ue
