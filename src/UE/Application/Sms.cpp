#include "Sms.hpp"

namespace ue
{

Sms::Sms(common::PhoneNumber from, const std::string& text, bool isRead)
    : from(from), text(text), isRead(isRead)
{
}

} // namespace ue
