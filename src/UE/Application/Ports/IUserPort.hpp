#pragma once

#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"
#include <vector>

namespace ue
{

class IUserEventsHandler
{
 public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleAccept() = 0;
    virtual void handleReject() = 0;
    virtual void handleMenuSelection(unsigned int index) = 0;
    virtual void handleDial(common::PhoneNumber to) = 0;
    virtual void handleSendCallDropped(common::PhoneNumber from) = 0;
    virtual void handleSendCallTalk(common::PhoneNumber to, const std::string &msg) = 0;
};

class IUserPort
{
 public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void displaySmsCompose() = 0;
    virtual Sms getSmsComposeData() = 0;
    virtual void displaySmsContent(Sms &sms) = 0;
    virtual void displaySmsList(const std::vector<Sms> &messages) = 0;
    virtual void showNewSms(bool present) = 0;

    virtual void showCallRequest(common::PhoneNumber from) = 0;
    virtual void showCallAccepted(common::PhoneNumber from) = 0;
    virtual void showCallDropped(common::PhoneNumber from) = 0;
    virtual void showCallDroppedAfterTalk(common::PhoneNumber from) = 0;
    virtual void showDialing() = 0;
    virtual void alertUser(std::string msg) = 0;
    virtual void setCallMode(common::PhoneNumber partnerPhoneNumber) = 0;
    virtual void newCallMessage(const std::string &text) = 0;
    virtual void waitingForCallRespond(common::PhoneNumber to) = 0;
};

} // namespace ue
