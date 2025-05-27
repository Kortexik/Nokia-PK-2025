#include "QtCallMode.hpp"

namespace ue
{

QtCallMode::QtCallMode(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget)
    : QtUeModeWidget(phoneNumberEdit, stackedWidget)
{
    constructGUI();
    connectSignals();
}

void QtCallMode::constructGUI()
{
    addChildWidget(&outgoingTextEdit);
    addChildWidget(&incomingTextEdit);

    // TODO: temp
    incomingTextEdit.setText("Outgoing text");
    incomingTextEdit.setText("Incoming text");
    //
    incomingTextEdit.setReadOnly(true);

    incomingTextEdit.show();
    outgoingTextEdit.show();
}

void QtCallMode::connectSignals()
{
    connect(&outgoingTextEdit, &QtSubmitTextEdit::submitted, [this]() { emit textEntered(); });
    connect(this, SIGNAL(appendTalkTextSignal(QString)), this, SLOT(appendTalkTextSlot(QString)));
    connect(this, SIGNAL(activateForDialModeSignal()), this, SLOT(activateForDialModeSlot()));
}

void QtCallMode::activateSlot()
{
    outgoingTextEdit.setEnabled(true);
    activateWithPhoneNumberEditDisabled();
}

void QtCallMode::activateForDialModeSlot()
{
    outgoingTextEdit.setEnabled(false);
    activateWithPhoneNumberEditEnabled();
}

void QtCallMode::clearIncomingText()
{
    incomingTextEdit.clear();
}

void QtCallMode::appendIncomingText(const std::string &text)
{
    emit appendTalkTextSignal(QString::fromStdString(text));
}

void QtCallMode::clearOutgoingText()
{
    outgoingTextEdit.clear();
}

std::string QtCallMode::getOutgoingText() const
{
    return outgoingTextEdit.toPlainText().toStdString();
}

void QtCallMode::activateForDialMode()
{
    emit activateForDialModeSignal();
}

void QtCallMode::appendTalkTextSlot(QString text)
{
    incomingTextEdit.append(text);
}

} // namespace ue
