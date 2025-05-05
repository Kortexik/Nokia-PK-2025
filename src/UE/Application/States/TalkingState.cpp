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

void TalkingState::handleCallAccepted(common::PhoneNumber from)
{
    
}

void TalkingState::handleCallDropped(common::PhoneNumber from)
{
    context.user.showCallDropped(from);
    context.setState<ConnectedState>();
}

void TalkingState::handleCallTalk(common::PhoneNumber from, std::string text)
{
    context.user.showIncomingText(from, text);
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(callingNumber);
    context.setState<ConnectedState>();
} 

}
