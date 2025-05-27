#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include "SmsDb.hpp"
#include <memory>

namespace ue
{

struct Context
{
    common::ILogger &logger;
    IBtsPort &bts;
    IUserPort &user;
    ITimerPort &timer;
    SmsDb smsDb;
    std::unique_ptr<IEventsHandler> state;
    common::PhoneNumber phoneNumber;

    template <typename State, typename... Arg> void setState(Arg &&...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

} // namespace ue
