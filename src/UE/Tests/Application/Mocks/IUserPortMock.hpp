#pragma once

#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock.h>
#include "Messages/PhoneNumber.hpp"
#include "Ports/IUserPort.hpp"

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
    MOCK_METHOD(void, showDialing, (), (final));
};
}
