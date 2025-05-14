#pragma once

#include "Messages/PhoneNumber.hpp"
#include <string>

namespace ue
{

struct Sms
{
    common::PhoneNumber from;
    std::string text;
    bool isRead;

    Sms(common::PhoneNumber from, const std::string& text, bool isRead = false);
};

} // namespace ue
