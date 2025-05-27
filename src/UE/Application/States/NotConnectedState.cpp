#include "NotConnectedState.hpp"
#include "ConnectingState.hpp"

namespace ue
{

NotConnectedState::NotConnectedState(Context &context) : BaseState(context, "NotConnectedState")
{
    context.user.showNotConnected();
}

void NotConnectedState::handleAccept()
{
    logger.logDebug("Accept pressed in NotConnectedState - ignoring");
}

void NotConnectedState::handleReject()
{
    logger.logDebug("Reject pressed in NotConnectedState - ignoring");
}

void NotConnectedState::handleDial(common::PhoneNumber to)
{
    logger.logDebug("Dial pressed in NotConnectedState - ignoring");
}

void NotConnectedState::handleSib(common::BtsId btsId)
{
    logger.logDebug("Received SIB: ", btsId);
    context.setState<ConnectingState>(btsId);
}

void NotConnectedState::handleAttachAccept()
{
    logger.logError("Unexpected attach accept in NotConnectedState");
}

void NotConnectedState::handleAttachReject()
{
    logger.logError("Unexpected attach reject in NotConnectedState");
}

} // namespace ue
