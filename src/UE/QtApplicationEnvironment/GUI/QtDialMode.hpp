#pragma once

#include "UeGui/IDialMode.hpp"

#include "Logger/ILogger.hpp"

namespace ue
{

class QtCallMode;
class QtPhoneNumberEdit;

using ILogger = common::ILogger;

class QtDialMode : public IUeGui::IDialMode
{
 public:
    QtDialMode(QtCallMode &baseMode, QtPhoneNumberEdit &phoneNumberEdit);
    ~QtDialMode();

    void init();
    void activate();

    PhoneNumber getPhoneNumber() const override;

 private:
    QtCallMode &baseMode;
    QtPhoneNumberEdit &phoneNumberEdit;
};
} // namespace ue
