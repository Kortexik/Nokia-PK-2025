#include "Sms.hpp"
#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{
Sms::Status Sms::getStatus() const
{
    return status;
}
Sms::Direction Sms::getDirection() const
{
    return direction;
}
std::string Sms::getText() const
{
    return text;
}
common::PhoneNumber Sms::getPhoneNumber() const
{
    return phoneNumber;
}

void Sms::setDirection(Sms::Direction direction)
{
    this->direction = direction;
}

void Sms::setText(const std::string &text)
{
    this->text = text;
}

void Sms::setStatus(Sms::Status status)
{
    this->status = status;
}

void Sms::setPhoneNumber(common::PhoneNumber phoneNumber)
{
    this->phoneNumber = phoneNumber;
}

} // namespace ue
