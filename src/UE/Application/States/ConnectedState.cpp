#include "ConnectedState.hpp"

#include "NotConnectedState.hpp"

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

void ConnectedState::handleSmsReceived(common::PhoneNumber from, const std::string& message)
{
    int index = context.smsDb.addSms(from, message);
    logger.logInfo("Received SMS from:", from, "with message:", message);
    context.user.showSms();
}


}
