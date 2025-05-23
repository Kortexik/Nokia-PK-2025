#include "SmsDb.hpp"
#include "Sms.hpp"

namespace ue
{

int SmsDb::addRecievedSms(common::PhoneNumber sourceNumber, const std::string& message)
{
    messages.push_back(Sms(sourceNumber, message, Sms::Status::UNREAD));
    return messages.size() - 1;
}

const std::vector<Sms>& SmsDb::viewAllSms() const
{
    return messages;
}

} // namespace ue


