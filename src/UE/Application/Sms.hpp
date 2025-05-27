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
    };
    enum class Direction : uint8_t
    {
        OUT,
        IN
    };

    Sms(common::PhoneNumber relativeNumber, const std::string &text, Status status)
        : phoneNumber(relativeNumber), text(text), status(status)
    {
    }
    Status getStatus() const;
    Direction getDirection() const;
    std::string getText() const;
    common::PhoneNumber getPhoneNumber() const;

    void setDirection(Direction direction);
    void setText(const std::string &text);
    void setStatus(Status status);
    void setPhoneNumber(common::PhoneNumber phoneNumber);

 private:
    common::PhoneNumber phoneNumber;
    std::string text;
    Status status;
    Direction direction;
};

} // namespace ue
