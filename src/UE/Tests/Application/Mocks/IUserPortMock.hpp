#pragma once

#include "Messages/PhoneNumber.hpp"
#include "Ports/IUserPort.hpp"
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock.h>

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
 public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleAccept, (), (final));
    MOCK_METHOD(void, handleReject, (), (final));
    MOCK_METHOD(void, handleDial, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, handleSendCallDropped, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, handleSendCallTalk, (common::PhoneNumber to, const std::string &msg), (final));
    MOCK_METHOD(void, handleMenuSelection, (unsigned int index), (final));
};

class IUserPortMock : public IUserPort
{
 public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));

    MOCK_METHOD(void, showCallRequest, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showCallAccepted, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showCallDropped, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showCallDroppedAfterTalk, (common::PhoneNumber from), (final));
    MOCK_METHOD(void, showDialing, (), (final));
    MOCK_METHOD(void, alertUser, (std::string msg), (final));
    MOCK_METHOD(void, setCallMode, (common::PhoneNumber partnerPhoneNumber), (final));
    MOCK_METHOD(void, newCallMessage, (const std::string &text), (final));
    MOCK_METHOD(void, waitingForCallRespond, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, showNewSms, (bool present), (final));
    MOCK_METHOD(void, displaySmsCompose, (), (final));
    MOCK_METHOD(Sms, getSmsComposeData, (), (final));
    MOCK_METHOD(void, displaySmsContent, (Sms & sms), (final));
    MOCK_METHOD(void, displaySmsList, (const std::vector<Sms> &messages), (final));
};
} // namespace ue
