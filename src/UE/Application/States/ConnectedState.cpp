#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}
void ConnectedState::handleDisconnected() {
    context.setState<NotConnectedState>();
}

void ConnectedState::handleTimeout()
{
    if (callingNumber.isValid()) {
        logger.logDebug("Call request timed out");
        context.bts.sendCallDrop(callingNumber);
        context.user.showCallDropped(callingNumber);
        callingNumber = common::PhoneNumber{};
        context.user.showConnected();
    }
}

void ConnectedState::handleAccept()
{
    context.timer.stopTimer();
    context.bts.sendCallAccepted(callingNumber);
    context.setState<TalkingState>(callingNumber);
}

void ConnectedState::handleReject()
{
        logger.logDebug("Rejecting call from: ", callingNumber);
        context.timer.stopTimer();
        context.bts.sendCallDrop(callingNumber);
        callingNumber = common::PhoneNumber{};
        context.user.showConnected();
}

void ConnectedState::handleMenuSelection(const std::string& selection)
{
    if (selection == "Call")
    {
        logger.logDebug("Call Selected From Menu");
        context.user.showDialing();
    }
}

void ConnectedState::handleDial(common::PhoneNumber to)
{
    logger.logDebug("Dialing", to);
    callingNumber = to;
    context.user.showDialing();
    context.bts.sendCallRequest(to);
    context.timer.startTimer(std::chrono::milliseconds(30000));
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
    {
        if (!callingNumber.isValid()) {
            callingNumber = from;
            context.user.showCallRequest(from);
            context.timer.startTimer(std::chrono::milliseconds(30000)); 
        }
        else {
            context.bts.sendCallDrop(from);
        }
    }

void ConnectedState::handleCallDropped(common::PhoneNumber from) {
    logger.logDebug("Call rejected by: ", from);
    context.user.showCallDropped(from);
    callingNumber = common::PhoneNumber{};
}
}
