#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber callingNumber)
    : BaseState(context, "TalkingState"),
      callingNumber(callingNumber)
{
    context.user.showCallAccepted(callingNumber);
}

void TalkingState::handleCallRequest(common::PhoneNumber from)
{
    context.bts.sendCallDrop(from);
}

void TalkingState::handleCallDropped(common::PhoneNumber from)
{
    context.user.showCallDropped(from);
    context.setState<ConnectedState>();
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(callingNumber);
    context.user.showCallDropped(callingNumber);
    context.setState<ConnectedState>();
}
void TalkingState::handleAccept()
{
    logger.logDebug("Accept pressed during call");
}

void TalkingState::handleReject()
{
    logger.logDebug("Reject pressed - ending call");
    context.bts.sendCallDrop(callingNumber);
    context.user.showCallDropped(callingNumber);
    context.setState<ConnectedState>();
}

void TalkingState::handleDial(common::PhoneNumber to)
{
    logger.logDebug("Dial pressed during call - ignoring");
}
}
