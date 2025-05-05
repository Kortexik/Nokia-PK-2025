#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;

    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleCallTalk(common::PhoneNumber from, std::string text) override;

    void handleAccept() override;
    void handleReject() override;
    void handleDial(common::PhoneNumber to) override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
