#include "UserPort.hpp"

#include "SmsDb.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <Messages/MessageId.hpp>
#include <UeGui/ISmsComposeMode.hpp>
#include <chrono>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"), gui(gui), phoneNumber(phoneNumber)
{
}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    callMode = nullptr;
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Call", "");

    gui.setAcceptCallback([this, &menu]() {
        auto selectedItem = menu.getCurrentItemIndex();
        bool exists = selectedItem.first;
        unsigned int selectedIndex = selectedItem.second;

        if (!exists)
        {
            return;
        }

        if (handler)
            handler->handleMenuSelection(selectedIndex);
    });
}

void UserPort::showNewSms(bool present)
{
    logger.logInfo("Showing new SMS - " + present);
    gui.showNewSms(present);
}

void UserPort::displaySmsCompose()
{
    logger.logInfo("Displaying SMS compose mode");
    IUeGui::ISmsComposeMode &compose = gui.setSmsComposeMode();

    compose.clearSmsText();

    gui.setAcceptCallback([this] {
        if (handler)
            handler->handleAccept();
    });
    gui.setRejectCallback([this] {
        if (handler)
            handler->handleReject();
    });
}

Sms UserPort::getSmsComposeData()
{
    IUeGui::ISmsComposeMode &compose = gui.setSmsComposeMode();

    common::PhoneNumber number = compose.getPhoneNumber();
    const std::string text = compose.getSmsText();
    Sms sms{number, text, Sms::Status::SENT};

    compose.clearSmsText();

    return sms;
}

void UserPort::displaySmsList(const std::vector<Sms> &messages)
{
    logger.logInfo("Displaying SMS list with length: ", messages.size());
    IUeGui::IListViewMode &menu = gui.setListViewMode();
    menu.clearSelectionList();
    for (auto it = messages.rbegin(); it != messages.rend(); ++it)
    {
        std::string messageStatus = (it->getStatus() == Sms::Status::UNREAD) ? "NEW! - " : "";
        std::string messageDirection = (it->getDirection() == Sms::Direction::IN) ? "From: " : "To: ";

        menu.addSelectionListItem(messageStatus + messageDirection + common::to_string(it->getPhoneNumber()), "");
    }

    gui.setAcceptCallback([this, &menu]() {
        auto selectedItem = menu.getCurrentItemIndex();
        bool exists = selectedItem.first;
        unsigned int selectedIndex = selectedItem.second;

        if (!exists)
        {
            return;
        }

        if (handler)
            handler->handleMenuSelection(selectedIndex);
    });

    gui.setRejectCallback([this] {
        if (handler)
            handler->handleReject();
    });
}

void UserPort::displaySmsContent(Sms &sms)
{
    IUeGui::ITextMode &view = gui.setViewTextMode();
    std::string messageDirection = (sms.getDirection() == Sms::Direction::IN) ? "From: " : "To: ";
    std::string messageText = sms.getText();
    std::string text = messageDirection + common::to_string(sms.getPhoneNumber()) + "\n\n" + messageText;

    view.setText(text);
}

void UserPort::showCallRequest(common::PhoneNumber from)
{
    auto &alertMode = gui.setAlertMode();
    alertMode.setText("Incoming call from: " + std::to_string(from.value));

    gui.setAcceptCallback([this]() {
        if (handler)
            handler->handleAccept();
    });

    gui.setRejectCallback([this]() {
        if (handler)
            handler->handleReject();
    });
}

void UserPort::showCallAccepted(common::PhoneNumber from)
{
    IUeGui::ICallMode &callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.clearOutgoingText();

    gui.setAcceptCallback([this, &callMode]() { auto outgoingText = callMode.getOutgoingText(); });

    gui.setRejectCallback([this]() {
        logger.logDebug("Call ended by user");
        if (handler)
        {
            handler->handleReject();
        }
    });

    logger.logDebug("Connected with: ", from.value);
}

void UserPort::showCallDropped(common::PhoneNumber from)
{
    auto &alertMode = gui.setAlertMode();
    alertMode.setText(std::to_string(from.value) + " is busy.");

    gui.setAcceptCallback([this]() { showConnected(); });
}

void UserPort::showCallDroppedAfterTalk(common::PhoneNumber from)
{
    auto &alertMode = gui.setAlertMode();
    alertMode.setText("Call dropped by " + std::to_string(from.value));

    gui.setAcceptCallback([this]() { showConnected(); });
}

void UserPort::showDialing()
{
    auto &dialMode = gui.setDialMode();

    gui.setAcceptCallback([this, &dialMode]() {
        auto enteredNumber = dialMode.getPhoneNumber();
        if (handler && enteredNumber.isValid())
        {
            handler->handleDial(enteredNumber);
        }
    });

    gui.setRejectCallback([this]() {
        if (handler)
        {
            handler->handleReject();
        }
    });
}

void UserPort::alertUser(std::string msg)
{

    auto &alert = gui.setAlertMode();
    alert.setText((msg));
    gui.setAcceptCallback([=]() { showConnected(); });
    gui.setRejectCallback([=]() { showConnected(); });
}

void UserPort::setCallMode(common::PhoneNumber partnerPhoneNumber)
{
    callMode = &gui.setCallMode();

    callMode->clearIncomingText();
    callMode->clearOutgoingText();

    gui.setAcceptCallback([=]() {
        auto text = callMode->getOutgoingText();
        if (!(text.empty()))
        {
            logger.logInfo("setCallMode  " + to_string(partnerPhoneNumber));
            handler->handleSendCallTalk(partnerPhoneNumber, text);
            callMode->clearOutgoingText();
        }
    });
    gui.setRejectCallback([=]() { handler->handleSendCallDropped(partnerPhoneNumber); });
}

void UserPort::waitingForCallRespond(common::PhoneNumber to)
{
    auto &alert = gui.setAlertMode();
    alert.setText("Calling " + std::to_string(to.value) + "...");
}

void UserPort::newCallMessage(const std::string &text)
{
    callMode->clearIncomingText();
    callMode->appendIncomingText(text);
}

} // namespace ue
