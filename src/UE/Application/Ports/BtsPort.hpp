#pragma once

#include "IBtsPort.hpp"
#include "ITransport.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
 public:
    BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler &handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    void sendCallAccepted(common::PhoneNumber to) override;
    void sendCallDrop(common::PhoneNumber to) override;
    void sendCallTalk(common::PhoneNumber to, const std::string &text) override;
    void sendCallRequest(common::PhoneNumber to) override;
    void sendSmsMessage(common::PhoneNumber to, const std::string &text) override;

 private:
    void handleDisconnected() const;
    void handleMessage(BinaryMessage msg);

    common::PrefixedLogger logger;
    common::ITransport &transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler *handler = nullptr;
};

} // namespace ue
