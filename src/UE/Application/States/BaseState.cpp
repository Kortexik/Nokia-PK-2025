#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState:: handleDisconnected() {
logger.logError("Uexpected: handleDisconnected");
}

void BaseState::handleSendCallDropped(common::PhoneNumber from) {
    logger.logError("Unexpected: handleSendCallDropped");
}

void BaseState::handleSendCallTalk(common::PhoneNumber to, const std::string &msg) {
    logger.logError("Unexpected : handleSendCallTalk ");
}

void BaseState::handleReceivedCallTalk(const std::string &text) {
    logger.logError("Unexpected : handleReceivedCallTalk ");
}
}
