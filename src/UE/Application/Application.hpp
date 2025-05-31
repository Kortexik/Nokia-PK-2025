#pragma once

#include "Context.hpp"
#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

using common::ILogger;
using common::PhoneNumber;

class Application : public IEventsHandler
{

public:
  Application(common::PhoneNumber phoneNumber,
          ILogger& iLogger,
          IBtsPort& bts,
          IUserPort& user,
          ITimerPort& timer,
          ISmsDb& smsDb);

    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;
    void handleSmsReceived(common::PhoneNumber from, const std::string &message) override;
    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleReceivedCallTalk(const std::string &text) override;

    // IUserEventsHandler interface
    void handleAccept() override;
    void handleReject() override;
    void handleMenuSelection(unsigned int index) override;
    void handleDial(common::PhoneNumber to) override;

    void handleSendCallDropped(common::PhoneNumber from) override;

    void handleSendCallTalk(common::PhoneNumber to, const std::string &msg) override;

 private:
    Context context;
    common::PrefixedLogger logger;
};

} // namespace ue
