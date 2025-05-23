#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"
#include "TalkingState.hpp"
namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() final;

    void handleCallRequest(common::PhoneNumber from) override;
    void handleAccept() override;
    void handleReject() override;

    void handleTimeout() override;

    void handleMenuSelection(const std::string& selection) override;

    void handleDial(common::PhoneNumber to) override;
    void handleCallDropped(common::PhoneNumber from) override;

    void handleCallAccepted(common::PhoneNumber from) override {
        context.timer.stopTimer();
        if (from == callingNumber) {
            context.setState<TalkingState>(from);
        }
    }
    void handleSendCallDropped(common::PhoneNumber from) final;
private:
    common::PhoneNumber callingNumber{};
};

}
