#pragma once

#include <QLineEdit>

#include "Messages/PhoneNumber.hpp"

namespace ue
{

using common::PhoneNumber;

class QtPhoneNumberEdit : public QLineEdit
{
    Q_OBJECT
 public:
    QtPhoneNumberEdit();
    ~QtPhoneNumberEdit();

    PhoneNumber getPhoneNumber() const;

    void enable();
    void disable();
};
} // namespace ue
