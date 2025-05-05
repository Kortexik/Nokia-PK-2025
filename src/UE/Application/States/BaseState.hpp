#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;


    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;

    virtual void handleCallRequest(common::PhoneNumber from)  override {
      context.bts.sendCallDrop(from);
    }
    virtual void handleCallAccepted(common::PhoneNumber) {}
    virtual void handleCallDropped(common::PhoneNumber) {}
    virtual void handleCallTalk(common::PhoneNumber, std::string) {}
    
    virtual void handleAccept()  override{
        logger.logDebug("Call accept ignored");
    }

    virtual void handleReject() override{
        logger.logDebug("Call Reject ignored");
    }
protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
