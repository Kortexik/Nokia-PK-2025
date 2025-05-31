#pragma once

#include "Logger/ILogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "QtPhoneNumberEdit.hpp"
#include "QtStackedWidget.hpp"
#include <QVBoxLayout>
#include <QWidget>

namespace ue
{

class QtUeModeWidget : public QWidget
{
    Q_OBJECT
 public:
    QtUeModeWidget(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget);

    void init();
    void activate();

 protected:
    void addChildWidget(QWidget *childWidget);

    void activateWithPhoneNumberEditEnabled();
    void activateWithPhoneNumberEditDisabled();

    PhoneNumber getPhoneNumber() const;

 private:
    void activateThisWidget();

    QVBoxLayout mainLayout;

    QtPhoneNumberEdit &phoneNumberEdit;
    QtStackedWidget &stackedWidget;
    int stackedWidgetIndex;

 signals:
    void activateSignal();

 protected slots:
    virtual void activateSlot() = 0;
};

} // namespace ue
