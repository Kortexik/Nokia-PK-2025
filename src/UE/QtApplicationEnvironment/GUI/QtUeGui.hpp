#pragma once

#include "IUeGui.hpp"

#include "QtAlertMode.hpp"
#include "QtCallMode.hpp"
#include "QtDialMode.hpp"
#include "QtMainWindow.hpp"
#include "QtPhoneNumberEdit.hpp"
#include "QtSelectionListMode.hpp"
#include "QtSmsComposeMode.hpp"
#include "QtStackedWidget.hpp"
#include "QtSubmitTextEdit.hpp"
#include "QtTextViewMode.hpp"

#include <QButtonGroup>
#include <QDesktopWidget>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Logger/ILogger.hpp"

namespace ue
{

using ILogger = common::ILogger;

class QtUeGui : public QObject, public IUeGui
{
    Q_OBJECT
 public:
    QtUeGui(ILogger &logger);
    ~QtUeGui();

    void start();

    void setCloseGuard(CloseGuard closeGuard) override;
    void setAcceptCallback(Callback) override;
    void setRejectCallback(Callback) override;

    void setTitle(const std::string &title) override;
    void showConnected() override;
    void showConnecting() override;
    void showNotConnected() override;
    void showNewSms(bool present) override;
    void showPeerUserNotAvailable(PhoneNumber peer) override;

    IListViewMode &setListViewMode() override;
    ISmsComposeMode &setSmsComposeMode() override;
    IDialMode &setDialMode() override;
    ICallMode &setCallMode() override;
    ITextMode &setAlertMode() override;
    ITextMode &setViewTextMode() override;

    int addModeWidget(QWidget *widget);

 signals:
    void phoneAcceptClicked();

    void setConnectedStateSignal(QString text, bool connected);
    void setNewMessageSignal(bool);
    void showInformationBoxSignal(QString, QString);

 private:
    // callbacks
    IUeGui::Callback acceptCallback;
    IUeGui::Callback rejectCallback;
    IUeGui::Callback homeCallback;

    void initGUI();
    void initInternalSignals();
    void initLayout();
    void addElements();
    void addPhoneNumberControls();
    void addButtons();
    void setButtonLayout(QPushButton &btn);
    void initModes();

    ILogger &logger;
    // part of main widget
    QtMainWindow mainWindow;
    QWidget centralWidget;
    QFrame frame;

    // here CALL mode widgets will be placed
    QtStackedWidget stackedWidget;

    QVBoxLayout mainLayout;

    // part of phone number layout
    QHBoxLayout phoneNumberLayout;
    QtPhoneNumberEdit phoneNumberEdit;
    QLabel connectedStateLabel;
    QLabel newMessageLabel;

    // navigate buttons
    QPushButton acceptButton;
    QPushButton rejectButton;
    QPushButton homeButton;

    QtCallMode callMode;
    QtDialMode dialMode;
    QtSelectionListMode listViewMode;
    QtSmsComposeMode smsComposeMode;
    QtAlertMode alertMode;
    QtTextViewMode textViewMode;

 private slots:
    void onAcceptClicked();
    void onRejectClicked();
    void onHomeClicked();
    void onItemSelected();
    void onTextEntered();
    void setConnectedStateSlot(QString text, bool connected);
    void setNewMessageSlot(bool);

 private:
    template <typename ModeObject> ModeObject &activateMode(ModeObject &modeObject);
};
} // namespace ue
