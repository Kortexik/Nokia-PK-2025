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

bool SmsDb::markAsRead(std::size_t index)
{
    if (index >= messages.size())
    {
        return false;
    }

    if (messages[index].getDirection() == Sms::Direction::IN && messages[index].getStatus() == Sms::Status::UNREAD)
    {
        messages[index].setStatus(Sms::Status::READ);
        return true;
    }
    return false;
}

bool SmsDb::markAsFailed(std::optional<std::size_t> index)
{
    if (index.has_value() && index.value() < messages.size())
    {
        auto &msg = messages[lastSentIndex.value()];
        if (msg.getDirection() == Sms::Direction::OUT)
        {
            msg.setStatus(Sms::Status::FAILED);
            lastSentIndex.reset();
            return true;
        }
    }
	return false;
}

std::vector<Sms>& SmsDb::getAllSms()
{
    return messages;
}

} // namespace ue


