#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context), logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}
void BaseState::handleDisconnected()
{
    logger.logError("Unexpected: handleDisconnected");
}

void BaseState::handleSmsReceived(common::PhoneNumber fromNumber, const std::string &message)
{
    logger.logError("Unexpected: handleSmsReceived from: ", fromNumber, " text: ", message);
}
void BaseState::handleSendCallDropped(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleSendCallDropped");
}

void BaseState::handleSendCallTalk(common::PhoneNumber to, const std::string &msg)
{
    logger.logError("Unexpected : handleSendCallTalk ");
}

void BaseState::handleMenuSelection(unsigned int index)
{
    logger.logError("Unexpected : handleMenuSelection ");
}

void BaseState::handleReceivedCallTalk(const std::string &text)
{
    logger.logError("Unexpected : handleReceivedCallTalk ");
}
} // namespace ue
