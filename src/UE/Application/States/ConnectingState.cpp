#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
namespace ue
{

ConnectingState::ConnectingState(Context &context)
    : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();
}

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    :BaseState(context,"ConnectingState")
{
    context.user.showConnecting();
    context.bts.sendAttachRequest(btsId);
    context.timer.startTimer(std::chrono::milliseconds(500));

}
    void ConnectingState::handleTimeout() {
        context.timer.stopTimer();
        context.setState<NotConnectedState>();
    }

void ConnectingState::handleAttachAccept(){
    context.timer.stopTimer();
    context.setState<ConnectedState>();
}
    void ConnectingState::handleAttachReject() {
        context.timer.stopTimer();
        context.setState<NotConnectedState>();
    }

}
