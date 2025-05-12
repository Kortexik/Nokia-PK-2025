#pragma once

#include "Sms.hpp"
#include <vector>
#include <cstddef>

namespace ue
{

class SmsDb
{
    std::vector<Sms> messages;

public:
    SmsDb() = default;

    int addSms(common::PhoneNumber from, const std::string& text);
    const std::vector<Sms>& viewAllSms() const;
};

} // namespace ue

