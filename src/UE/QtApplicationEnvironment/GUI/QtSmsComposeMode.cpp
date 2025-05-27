#include "QtSmsComposeMode.hpp"

namespace ue
{

QtSmsComposeMode::QtSmsComposeMode(QtPhoneNumberEdit &phoneNumberTextEdit, QtStackedWidget &stackedWidget)
    : QtUeModeWidget(phoneNumberTextEdit, stackedWidget)
{
    constructGUI();
    connectSignals();
}

void QtSmsComposeMode::constructGUI()
{
    addChildWidget(&smsTextEdit);
    smsTextEdit.show();
}

void QtSmsComposeMode::connectSignals()
{
    connect(this, SIGNAL(setSmsTextSignal(QString)), this, SLOT(setSmsTextSlot(QString)));
    connect(this, SIGNAL(clearSmsTextSignal()), this, SLOT(clearSmsTextSlot()));
    connect(this, SIGNAL(activateForViewingSmsSignal()), this, SLOT(activateForViewingSmsSlot()));
}

void QtSmsComposeMode::activateSlot()
{
    smsTextEdit.setEnabled(true);
    activateWithPhoneNumberEditEnabled();
}

void QtSmsComposeMode::activateForViewingSmsSlot()
{
    smsTextEdit.setEnabled(false);
    activateWithPhoneNumberEditDisabled();
}

void QtSmsComposeMode::setSmsTextSlot(QString value)
{
    smsTextEdit.setPlainText(value);
}

QString QtSmsComposeMode::getText() const
{
    return smsTextEdit.toPlainText();
}

PhoneNumber QtSmsComposeMode::getPhoneNumber() const
{
    return QtUeModeWidget::getPhoneNumber();
}

std::string QtSmsComposeMode::getSmsText() const
{
    return getText().toStdString();
}

void QtSmsComposeMode::clearSmsText()
{
    emit clearSmsTextSignal();
}

void QtSmsComposeMode::clearSmsTextSlot()
{
    smsTextEdit.clear();
}

void QtSmsComposeMode::activateForViewingSms()
{
    emit activateForViewingSmsSignal();
}

void QtSmsComposeMode::setSmsText(const std::string &value)
{
    emit setSmsTextSignal(QString::fromStdString(value));
}

} // namespace ue
