#pragma once

#include <gmock/gmock.h>

#include "IApplicationEnvironment.hpp"

namespace ue
{

struct IApplicationEnvironmentMock : public IApplicationEnvironment
{
    IApplicationEnvironmentMock();
    ~IApplicationEnvironmentMock() override;

    MOCK_METHOD(IUeGui &, getUeGui, (), (final));
    MOCK_METHOD(common::ITransport &, getTransportToBts, (), (final));
    MOCK_METHOD(common::ILogger &, getLogger, (), (final));
    MOCK_METHOD(void, startMessageLoop, (), (final));
    MOCK_METHOD(common::PhoneNumber, getMyPhoneNumber, (), (const, final));
    MOCK_METHOD(int32_t, getProperty, (const std::string &name, int32_t defaultValue), (const, final));
};

} // namespace ue
