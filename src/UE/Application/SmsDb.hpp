#pragma once

#include "Sms.hpp"
#include <vector>
#include <cstddef>
#include <optional>

namespace ue
{

class SmsDb
{
    std::vector<Sms> messages;
    std::optional<std::size_t> lastSentIndex;

public:
    SmsDb() = default;
    std::size_t addReceivedSms(common::PhoneNumber source, const std::string& text);
    std::size_t addSentSms(common::PhoneNumber to, const std::string& text);
    std::vector<Sms>& getAllSms();
    std::size_t getUnreadCount() const;
};

} // namespace ue

