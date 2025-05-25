#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showNewSms() override;
    void displaySmsList(const std::vector<Sms>& messages) override;
    void displaySmsContent(const Sms& message);
    void displayAlert(const std::string& title, const std::string& message);
    //common::PhoneNumber getSmsRecipient() const;
    //std::string getSmsText() const override;

    void showCallRequest(common::PhoneNumber from) override;
    void showCallAccepted(common::PhoneNumber from) override;
    void showCallDropped(common::PhoneNumber from) override;
    void showCallDroppedAfterTalk(common::PhoneNumber from) override;
    void showDialing() override;
    void alertUser(std::string msg) override;
    void setCallMode(common::PhoneNumber partnerPhoneNumber) override;
    void waitingForCallRespond(common::PhoneNumber to) override;
    void newCallMessage(const std::string &text) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    IUeGui::ICallMode* callMode= nullptr;
};

}
