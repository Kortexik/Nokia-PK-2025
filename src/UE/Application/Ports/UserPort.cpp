#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
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
    auto& textMode = gui.setViewTextMode();
    textMode.setText("Connected with: " + std::to_string(from.value));
}

void UserPort::showCallDropped(common::PhoneNumber from)
{
    auto& textMode = gui.setAlertMode(); 
    textMode.setText("Call with " + std::to_string(from.value) + " ended");

    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::showIncomingText(common::PhoneNumber from, const std::string& text)
{
    auto& textMode = gui.setViewTextMode();
    textMode.setText("From:  " + std::to_string(from.value) + ":\n" + text);
}

}
