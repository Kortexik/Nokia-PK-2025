#include "QtDialMode.hpp"
#include "QtCallMode.hpp"
#include "QtPhoneNumberEdit.hpp"

namespace ue
{

QtDialMode::QtDialMode(QtCallMode &baseMode, QtPhoneNumberEdit &phoneNumberEdit)
    : baseMode(baseMode), phoneNumberEdit(phoneNumberEdit)
{
}

QtDialMode::~QtDialMode()
{
}

void QtDialMode::init()
{
}

void QtDialMode::activate()
{
    baseMode.activateForDialMode();
}

PhoneNumber QtDialMode::getPhoneNumber() const
{
    return phoneNumberEdit.getPhoneNumber();
}

} // namespace ue
