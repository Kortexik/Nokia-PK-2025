#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() final;
    void handleSmsReceived(common::PhoneNumber from, const std::string& message) override;
    void handleCallRequest(common::PhoneNumber from) override;
    void handleAccept() override;
    void handleReject() override;
    void handleTimeout() override;
    void handleMenuSelection(unsigned int index) override;
    void handleDial(common::PhoneNumber to) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleCallAccepted(common::PhoneNumber from) override;
    void handleSendCallDropped(common::PhoneNumber from);
private:
    common::PhoneNumber callingNumber{};
};

}
