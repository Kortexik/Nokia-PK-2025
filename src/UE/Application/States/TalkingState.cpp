#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber callingNumber)
    : ConnectedState(context), callingNumber(callingNumber)
    {
        context.user.setCallMode(callingNumber);
        using namespace std::chrono_literals;
        context.timer.startTimer(120s);
    }

void TalkingState::handleCallRequest(common::PhoneNumber from)
{
    context.bts.sendCallDrop(from);
}

void TalkingState::handleCallDropped(common::PhoneNumber from)
{
    logger.logDebug("Call dropped by: ", from);
    context.setState<ConnectedState>();
    context.user.showCallDroppedAfterTalk(from);
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

void TalkingState::handleSendCallDropped(common::PhoneNumber from)
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(from);
    context.setState<ConnectedState>();
}

void TalkingState::handleSendCallTalk(common::PhoneNumber to, const std::string &msg)
{
    context.timer.stopTimer();
    context.bts.sendCallTalk(to,msg);
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
}

void TalkingState::handleReceivedCallTalk(const std::string &text)
{
    context.timer.stopTimer();
    context.user.newCallMessage(text);
    using namespace std::chrono_literals;
    context.timer.startTimer(120s);
}
}
