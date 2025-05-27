#include "QtAlertMode.hpp"
#include <QPalette>

namespace ue
{

QtAlertMode::QtAlertMode(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget)
    : QtUeModeWidget(phoneNumberEdit, stackedWidget)
{
    constructGUI();
    connectSignals();
}

void QtAlertMode::constructGUI()
{
    QPalette labelPalette{};
    labelPalette.setColor(QPalette::Dark, Qt::red);
    labelPalette.setColor(QPalette::WindowText, Qt::red);
    QFont labelFont("Arial Narrow", 20);

    alertText.setAlignment(Qt::AlignCenter);
    alertText.setPalette(labelPalette);
    alertText.setFont(labelFont);
    alertText.setFrameStyle(QFrame::Box | QFrame::Raised);

    addChildWidget(&alertText);
}

void QtAlertMode::connectSignals()
{
    QObject::connect(this, SIGNAL(setTextSignal(QString)), this, SLOT(setTextSlot(QString)));
}

void QtAlertMode::activateSlot()
{
    activateWithPhoneNumberEditDisabled();
}

void QtAlertMode::setText(const std::string &text)
{
    emit setTextSignal(QString::fromStdString(text));
}

void QtAlertMode::setTextSlot(QString text)
{
    alertText.setText(text);
}

} // namespace ue
