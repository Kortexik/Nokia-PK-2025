#pragma once

#include "Sms.hpp"
#include <vector>
#include <cstddef>

namespace ue
{

class ISmsDb
{
public:
    virtual ~ISmsDb() = default;

    virtual std::size_t addReceivedSms(common::PhoneNumber source, const std::string& text) = 0;
    virtual std::size_t addSentSms(common::PhoneNumber to, const std::string& text) = 0;
    virtual std::vector<Sms>& getAllSms() = 0;
    virtual std::size_t getUnreadCount() const = 0;
};

}

