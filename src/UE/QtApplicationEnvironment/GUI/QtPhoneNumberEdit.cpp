#include "QtPhoneNumberEdit.hpp"

#include <QFont>
#include <QIntValidator>
#include <QPalette>

namespace ue
{

QtPhoneNumberEdit::QtPhoneNumberEdit()
{
    setFont(QFont("Arial Narrow", 16));
    setValidator(new QIntValidator(PhoneNumber::MIN_VALUE, PhoneNumber::MAX_VALUE, this));
    setMaxLength(PhoneNumber::DIGITS);
    setStyleSheet("background-color:rgba( 255, 255, 255, 0% );");
}

QtPhoneNumberEdit::~QtPhoneNumberEdit()
{
}

PhoneNumber QtPhoneNumberEdit::getPhoneNumber() const
{
    bool ok = false;
    decltype(PhoneNumber::value) val = text().toUInt(&ok, 10);
    if (ok)
        return PhoneNumber{val};
    return PhoneNumber{};
}

void QtPhoneNumberEdit::enable()
{
    setEnabled(true);
    setStyleSheet("QLineEdit {background-color: #ccfb5d;}");
    setFocus();
}

void QtPhoneNumberEdit::disable()
{
    setEnabled(false);
    setStyleSheet("QLineEdit {background-color: #ababab;}");
}

} // namespace ue
