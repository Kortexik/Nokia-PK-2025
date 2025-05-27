#pragma once

#include "QtUeModeWidget.hpp"
#include "UeGui/ISmsComposeMode.hpp"

#include <QPlainTextEdit>

namespace ue
{

class QtSmsComposeMode : public QtUeModeWidget, public IUeGui::ISmsComposeMode
{
    Q_OBJECT
 public:
    QtSmsComposeMode(QtPhoneNumberEdit &phoneNumberEdit, QtStackedWidget &stackedWidget);

    PhoneNumber getPhoneNumber() const override;
    std::string getSmsText() const override;
    void clearSmsText() override;

    void activateForViewingSms();
    void setSmsText(const std::string &);

 private:
    void constructGUI();
    void connectSignals();
    QString getText() const;

    QPlainTextEdit smsTextEdit;

 signals:
    void activateForViewingSmsSignal();
    void setSmsTextSignal(QString);
    void clearSmsTextSignal();

 private slots:
    void activateSlot() override;
    void activateForViewingSmsSlot();
    void setSmsTextSlot(QString);
    void clearSmsTextSlot();
};

} // namespace ue
