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

void ConnectedState::handleCallAccepted(common::PhoneNumber from)
{
    // Handle if needed
}

void ConnectedState::handleCallDropped(common::PhoneNumber from)
{
    context.user.showCallDropped(from);
}

void ConnectedState::handleCallTalk(common::PhoneNumber from, std::string text)
{
    context.user.showIncomingText(from, text);
}

void ConnectedState::handleTimeout()
{
    context.user.showCallDropped(callingNumber);
    context.bts.sendCallDrop(callingNumber);
}

void ConnectedState::handleAcceptCall()
{
    context.timer.stopTimer();
    context.bts.sendCallAccepted(callingNumber);
    context.setState<TalkingState>(callingNumber);
}

void ConnectedState::handleRejectCall()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(callingNumber);
}
}
