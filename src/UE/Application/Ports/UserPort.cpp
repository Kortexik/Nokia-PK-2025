#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ICallMode.hpp"
#include <chrono>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

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
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Call", "");

    gui.setAcceptCallback([this, &menu]() {
        auto selectedItem = menu.getCurrentItemIndex();
        bool exists = selectedItem.first;
        unsigned int selectedIndex = selectedItem.second;

        if (!exists) {
            return;
        }
            
        switch (selectedIndex)
        {
        case 0:
            logger.logDebug("Handling Compose SMS not implemented");
            break;
        case 1:
            logger.logDebug("Handle View SMS not implemented");
            break;
        case 2:
            logger.logDebug("User selected: Call");
            showDialing();
            break;
        default:
            logger.logDebug("Default option selected, should be never");
            break;
        }
    });
}

void UserPort::showCallRequest(common::PhoneNumber from)
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Incoming call from: " + std::to_string(from.value));
    
    gui.setAcceptCallback([this]() {
        if (handler) handler->handleAccept();
    });
    
    gui.setRejectCallback([this]() {
        if (handler) handler->handleReject();
    });
}

void UserPort::showCallAccepted(common::PhoneNumber from)
{
    IUeGui::ICallMode& callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.clearOutgoingText();

    gui.setAcceptCallback([this, &callMode]() {
        auto outgoingText = callMode.getOutgoingText();
    });

    gui.setRejectCallback([this]() {
        logger.logDebug("Call ended by user");
        if (handler) {
            handler->handleReject();
        }
    });

    logger.logDebug("Connected with: ", from.value);
}

void UserPort::showCallDropped(common::PhoneNumber from)
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText(std::to_string(from.value) + " is busy.");

    gui.setAcceptCallback([this]() {
        showConnected();
    });
}

void UserPort::showCallDroppedAfterTalk(common::PhoneNumber from)
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Call dropped by " + std::to_string(from.value));

    gui.setAcceptCallback([this]() {
        showConnected();
    });
}

void UserPort::showDialing()
{
    auto& dialMode = gui.setDialMode();
    
    gui.setAcceptCallback([this, &dialMode]() {
        auto enteredNumber = dialMode.getPhoneNumber();
        if (handler && enteredNumber.isValid()) { 
            handler->handleDial(enteredNumber);
        }
    });
    
    gui.setRejectCallback([this]() {
        if (handler) {
            handler->handleReject();
        }
    });
}

void UserPort::alertUser(std::string msg) {

    auto &alert=gui.setAlertMode();
    alert.setText((msg));
    gui.setAcceptCallback([=](){showConnected();});
    gui.setRejectCallback([=](){showConnected();});
}

void UserPort::setCallMode(common::PhoneNumber partnerPhoneNumber) {
    callMode=&gui.setCallMode();

    callMode->clearIncomingText();
    callMode->clearOutgoingText();

    gui.setAcceptCallback([=](){
        auto text=callMode->getOutgoingText();
        if(!(text.empty())){
            logger.logInfo("setCallMode  " + to_string(partnerPhoneNumber));
            handler->handleSendCallTalk(partnerPhoneNumber,text);
            callMode->clearOutgoingText();
        }
    });
    gui.setRejectCallback([=](){handler->handleSendCallDropped(partnerPhoneNumber);});
}

void UserPort::waitingForCallRespond(common::PhoneNumber to) {
    auto &alert=gui.setAlertMode();
    alert.setText("Calling " + std::to_string(to.value) + "...");
}

void UserPort::newCallMessage(const std::string &text) {
    callMode->clearIncomingText();
    callMode->appendIncomingText(text);
}

}
