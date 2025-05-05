#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() final;

    void handleCallRequest(common::PhoneNumber from) override {
        callingNumber = from;
        context.user.showCallRequest(from);
        context.timer.startTimer(std::chrono::milliseconds(30000));
    }
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleCallTalk(common::PhoneNumber from, std::string text) override;
    
    void handleTimeout() override;
    void handleAcceptCall();
    void handleRejectCall();

    void handleMenuSelection(const std::string& selection) override;
    void handleDial(common::PhoneNumber to) override;
private:
    common::PhoneNumber callingNumber{};
};

}
