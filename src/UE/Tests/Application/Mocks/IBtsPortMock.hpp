#pragma once

#include "Ports/IBtsPort.hpp"
#include <gmock/gmock.h>

namespace ue
{

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
 public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleDisconnected, (), (final));
    MOCK_METHOD(void, handleCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallAccepted, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleCallDropped, (common::PhoneNumber), (final));
    MOCK_METHOD(void, handleReceivedCallTalk, (const std::string &text), (final));
    MOCK_METHOD(void, handleSmsReceived, (common::PhoneNumber fromNumber, const std::string &message), (override));
};

class IBtsPortMock : public IBtsPort
{
 public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(void, sendCallAccepted, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, sendCallDrop, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber to), (final));
    MOCK_METHOD(void, sendCallTalk, (common::PhoneNumber to, const std::string &text), (final));
    MOCK_METHOD(void, sendSmsMessage, (common::PhoneNumber to, const std::string &text), (final));
};

} // namespace ue
