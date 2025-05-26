#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class TalkingState : public ConnectedState
{
public:
    TalkingState(Context& context, common::PhoneNumber callingNumber);

    void handleAccept() override;
    void handleReject() override;
    void handleDial(common::PhoneNumber to) override;

    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleTimeout() override;

    void handleSendCallTalk(common::PhoneNumber to, const std::string &msg) override;
    void handleReceivedCallTalk(const std::string &text) override;
    void handleSendCallDropped(common::PhoneNumber from) override;
private:
    common::PhoneNumber callingNumber;
};

}
