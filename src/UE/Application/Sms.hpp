#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{

class Sms
{
public:
    enum class Status : uint8_t
    {
        READ,
        UNREAD,
        SENT,
        FAILED
    };

    Sms(common::PhoneNumber phoneNumber, const std::string& text, Status status) : sourceNumber(phoneNumber), text(text), status(status) {}

private:
    common::PhoneNumber sourceNumber;
    std::string text;
    Status status;
};

} // namespace ue
