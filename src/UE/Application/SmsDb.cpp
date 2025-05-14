#include "SmsDb.hpp"

namespace ue
{

int SmsDb::addSms(common::PhoneNumber fromNumber, const std::string& message)
{
    Sms newSms(fromNumber, message, false);
    messages.push_back(newSms);
    return messages.size() - 1;
}

const std::vector<Sms>& SmsDb::viewAllSms() const
{
    return messages;
}

} // namespace ue


