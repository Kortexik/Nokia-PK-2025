#include "QtUeGui.hpp"

#include <QApplication>
#include <QPalette>

namespace ue
{

QtUeGui::QtUeGui(ILogger &logger)
    : logger(logger), mainWindow(), centralWidget(&mainWindow), frame(&centralWidget), acceptButton(&centralWidget),
      rejectButton(&centralWidget), homeButton(&centralWidget), listViewMode(phoneNumberEdit, stackedWidget),
      callMode(phoneNumberEdit, stackedWidget), dialMode(callMode, phoneNumberEdit),
      smsComposeMode(phoneNumberEdit, stackedWidget), alertMode(phoneNumberEdit, stackedWidget),
      textViewMode(smsComposeMode)
{
    initGUI();
    initInternalSignals();
}

QtUeGui::~QtUeGui()
{
}

void QtUeGui::start()
{
    mainWindow.show();
}

void QtUeGui::initGUI()
{
    // TODO: change the fixed sizes into relative values
    QSize cellPhoneSize(400, 750);
    mainWindow.setFixedSize(cellPhoneSize);
    mainWindow.setWindowTitle("UE <no-number>");

    // move to the center
    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.move(QApplication::desktop()->screen()->rect().center() - mainWindow.rect().center());

    // set background to centralWidget
    centralWidget.setObjectName("centralWidget");
    centralWidget.setStyleSheet("#centralWidget{"
                                "border-image:url(\"images/cell_phone.png\") "
                                "0 0 0 0 stretch stretch;}");

    // init stacked widget
    frame.setFrameStyle(QFrame::Box | QFrame::Raised);
    frame.setFixedSize(cellPhoneSize.width() * 307 / 412, cellPhoneSize.height() * 540 / 800);
    frame.move(cellPhoneSize.width() * 53 / 412, cellPhoneSize.height() * 90 / 800);

    initLayout();
    addElements();
    initModes();
}

void QtUeGui::initInternalSignals()
{
    QObject::connect(&acceptButton, SIGNAL(clicked()), this, SLOT(onAcceptClicked()));
    QObject::connect(&rejectButton, SIGNAL(clicked()), this, SLOT(onRejectClicked()));
    QObject::connect(&homeButton, SIGNAL(clicked()), this, SLOT(onHomeClicked()));

    QObject::connect(&listViewMode, SIGNAL(itemDoubleClicked()), this, SLOT(onItemSelected()));
    QObject::connect(&callMode, SIGNAL(textEntered()), this, SLOT(onTextEntered()));

    QObject::connect(this, SIGNAL(setConnectedStateSignal(QString, bool)), this,
                     SLOT(setConnectedStateSlot(QString, bool)));
    QObject::connect(this, SIGNAL(setNewMessageSignal(bool)), this, SLOT(setNewMessageSlot(bool)));
}

void QtUeGui::initLayout()
{
    phoneNumberLayout.setContentsMargins(2, 2, 2, 2);
    mainLayout.setContentsMargins(2, 2, 2, 2);

    mainLayout.addLayout(&phoneNumberLayout);
    mainLayout.addWidget(&stackedWidget);

    frame.setLayout(&mainLayout);
}

void QtUeGui::addElements()
{
    addPhoneNumberControls();
    addButtons();
}

void QtUeGui::addPhoneNumberControls()
{
    phoneNumberEdit.setMaximumHeight(30);
    phoneNumberEdit.setMinimumHeight(30);
    // phoneNumberTextEdit.viewport()->setAutoFillBackground(false);

    connectedStateLabel.setFont(QFont("Arial Narrow", 12, QFont::Bold));
    connectedStateLabel.setText(" C");
    setConnectedStateSlot("", false);
    newMessageLabel.setFont(QFont("Arial Narrow", 12, QFont::Bold));
    newMessageLabel.setText(" M ");
    setNewMessageSlot(false);

    phoneNumberLayout.addWidget(&connectedStateLabel);
    phoneNumberLayout.addWidget(&newMessageLabel);
    phoneNumberLayout.addWidget(&phoneNumberEdit);
}

void QtUeGui::addButtons()
{
    int buttonsCOmmonY = 620;

    // TODO: remove duplicated code

    setButtonLayout(acceptButton);
    setButtonLayout(rejectButton);
    setButtonLayout(homeButton);

    acceptButton.setIcon(QIcon(QPixmap("images/call_acc.png")));
    acceptButton.move(70, buttonsCOmmonY);

    homeButton.setIcon(QIcon(QPixmap("images/sms.png")));
    homeButton.move(176, buttonsCOmmonY);

    rejectButton.setIcon(QIcon(QPixmap("images/call_rej.png")));
    rejectButton.move(290, buttonsCOmmonY);
}

void QtUeGui::setButtonLayout(QPushButton &btn)
{
    QSize stdBtnSize(48, 48);

    btn.setMinimumSize(stdBtnSize);
    btn.setMaximumSize(stdBtnSize);
    btn.setIconSize(stdBtnSize);
    btn.setStyleSheet("background-color: rgba( 255, 255, 255, 0% );");
}

void QtUeGui::initModes()
{
    alertMode.init();
    smsComposeMode.init();
    listViewMode.init();
    callMode.init();
    dialMode.init();
    textViewMode.init();
}

void QtUeGui::onAcceptClicked()
{
    if (acceptCallback)
    {
        acceptCallback();
    }
}

void QtUeGui::onRejectClicked()
{
    if (rejectCallback)
    {
        rejectCallback();
    }
}

void QtUeGui::onHomeClicked()
{
    if (homeCallback)
    {
        homeCallback();
    }
}

void QtUeGui::onItemSelected()
{
    onAcceptClicked();
}

void QtUeGui::onTextEntered()
{
    onAcceptClicked();
}

void QtUeGui::setCloseGuard(CloseGuard closeGuard)
{
    mainWindow.setCloseGuard(std::move(closeGuard));
}

void QtUeGui::setAcceptCallback(Callback callback)
{
    acceptCallback = callback;
}

void QtUeGui::setRejectCallback(Callback callback)
{
    rejectCallback = callback;
}

void QtUeGui::setTitle(const std::string &title)
{
    mainWindow.setWindowTitle(QString::fromStdString(title));
}

void QtUeGui::showConnected()
{
    setAlertMode().setText("Connected");
    emit setConnectedStateSignal("Connected", true);
}

void QtUeGui::showConnecting()
{
    setAlertMode().setText("Connecting");
    ;
    emit setConnectedStateSignal("Connecting", false);
}

void QtUeGui::showNotConnected()
{
    setAlertMode().setText("Not connected");
    emit setConnectedStateSignal("Not connected", false);
}

void QtUeGui::showNewSms(bool present)
{
    emit setNewMessageSignal(present);
}

void QtUeGui::showPeerUserNotAvailable(PhoneNumber peer)
{
    setAlertMode().setText("Not available: " + to_string(peer));
}

void QtUeGui::setConnectedStateSlot(QString text, bool connected)
{
    logger.logDebug("GUI: setConnectedStateSlot(", text.toStdString(), ")");
    connectedStateLabel.setEnabled(connected);
    connectedStateLabel.setToolTip(text);
}

void QtUeGui::setNewMessageSlot(bool value)
{
    logger.logDebug("GUI: setNewMessageSlot(", value, ")");
    newMessageLabel.setEnabled(value);
    newMessageLabel.setToolTip(value ? "New message(s)" : "No new messages");
}

template <typename ModeObject> ModeObject &QtUeGui::activateMode(ModeObject &modeObject)
{
    modeObject.activate();
    return modeObject;
}

IUeGui::IListViewMode &QtUeGui::setListViewMode()
{
    return activateMode(listViewMode);
}

IUeGui::ISmsComposeMode &QtUeGui::setSmsComposeMode()
{
    return activateMode(smsComposeMode);
}

IUeGui::IDialMode &QtUeGui::setDialMode()
{
    return activateMode(dialMode);
}

IUeGui::ICallMode &QtUeGui::setCallMode()
{
    return activateMode(callMode);
}

IUeGui::ITextMode &QtUeGui::setAlertMode()
{
    return activateMode(alertMode);
}

IUeGui::ITextMode &QtUeGui::setViewTextMode()
{
    return activateMode(textViewMode);
}

} // namespace ue
