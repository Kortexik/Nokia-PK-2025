#pragma once

#include "Sms.hpp"
#include "ISmsDb.hpp"
#include <vector>
#include <optional>

namespace ue
{

class SmsDb : public ISmsDb
{
    std::vector<Sms> messages;
    std::optional<std::size_t> lastSentIndex;

public:
    SmsDb() = default;

    std::size_t addReceivedSms(common::PhoneNumber source, const std::string& text) override;
    std::size_t addSentSms(common::PhoneNumber to, const std::string& text) override;
    std::vector<Sms>& getAllSms() override;
    std::size_t getUnreadCount() const override;
};

}


