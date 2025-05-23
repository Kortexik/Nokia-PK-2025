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

    int addRecievedSms(common::PhoneNumber source, const std::string& text);
    int addSentSms(common::PhoneNumber to, const std::string& text);
    const std::vector<Sms>& viewAllSms() const;
};

} // namespace ue

