#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "SmsComposeState.hpp"
#include "SmsListState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.user.showNewSms(context.smsDb.getUnreadCount() > 0);
}
void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSmsReceived(common::PhoneNumber from, const std::string& message)
{
    context.smsDb.addReceivedSms(from, message);
    logger.logInfo("Received SMS from:", from, "with message:", message);
    context.user.showNewSms(context.smsDb.getUnreadCount() > 0);
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

void ConnectedState::handleMenuSelection(unsigned int index)
{
    switch (index)
    {
    case 0:
        context.setState<SmsComposeState>();
        break;
    case 1:
        context.setState<SmsListState>();
        break;
    case 2:
        logger.logInfo("Call selected");
        context.user.showDialing();
        break;
    default:
        logger.logDebug("Default option selected, should be never");
        break;
    }
}

void ConnectedState::handleDial(common::PhoneNumber to)
{
    logger.logDebug("Dialing ", to);
    callingNumber = to;
    context.user.showDialing();
    context.bts.sendCallRequest(to);
    context.user.waitingForCallRespond(to);
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

void ConnectedState::handleSendCallDropped(common::PhoneNumber from){
    context.timer.stopTimer();
    context.bts.sendCallDrop(from);
    context.user.showConnected();
}
}
