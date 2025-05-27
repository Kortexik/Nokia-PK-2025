#include "QtUeModeWidget.hpp"

namespace ue
{

QtUeModeWidget::QtUeModeWidget(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget)
    : phoneNumberEdit(phoneNumberEdit), stackedWidget(stackedWidget)
{
    setLayout(&mainLayout);
    connect(this, SIGNAL(activateSignal()), this, SLOT(activateSlot()));
}

void QtUeModeWidget::addChildWidget(QWidget *childWidget)
{
    mainLayout.addWidget(childWidget);
}

void QtUeModeWidget::activateWithPhoneNumberEditEnabled()
{
    phoneNumberEdit.enable();
    activateThisWidget();
}

void QtUeModeWidget::activateWithPhoneNumberEditDisabled()
{
    phoneNumberEdit.disable();
    activateThisWidget();
}

PhoneNumber QtUeModeWidget::getPhoneNumber() const
{
    return phoneNumberEdit.getPhoneNumber();
}

void QtUeModeWidget::activateThisWidget()
{
    stackedWidget.setCurrentIndex(stackedWidgetIndex);
}

void QtUeModeWidget::init()
{
    stackedWidgetIndex = stackedWidget.addModeWidget(this);
}

void QtUeModeWidget::activate()
{
    emit activateSignal();
}

} // namespace ue
