#include "SmsDb.hpp"
#include "Sms.hpp"
#include <optional>

namespace ue
{

std::size_t SmsDb::addReceivedSms(common::PhoneNumber source, const std::string& message)
{
    Sms sms = Sms(source, message, Sms::Status::UNREAD);
    sms.setDirection(Sms::Direction::IN);
    messages.push_back(sms);
    return messages.size() - 1;
}

std::size_t SmsDb::addSentSms(common::PhoneNumber to, const std::string& message)
{
    Sms sms = Sms(to, message, Sms::Status::SENT);
    sms.setDirection(Sms::Direction::OUT);
    messages.push_back(sms);
    lastSentIndex = messages.size() - 1;
    return messages.size() - 1;
}

std::size_t SmsDb::getUnreadCount() const
{
    std::size_t amount = 0;
    for(Sms message : messages)
    {
        if(message.getStatus() == Sms::Status::UNREAD)
        {
            amount++;
        }
    }
    return amount;
}

std::vector<Sms>& SmsDb::getAllSms()
{
    return messages;
}

} // namespace ue


