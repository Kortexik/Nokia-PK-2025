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
    const std::vector<Sms>& getAllSms() const;
    std::size_t getUnreadCount() const;
    bool markAsRead(std::size_t index);
    bool markAsFailed(std::optional<std::size_t> index);
};

} // namespace ue

