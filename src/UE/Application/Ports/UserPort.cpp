#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ICallMode.hpp"

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
    
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
    
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

    gui.setAcceptCallback([this, &callMode, from]() {
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
    alertMode.setText("Call with " + std::to_string(from.value) + " ended");
    
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

}
