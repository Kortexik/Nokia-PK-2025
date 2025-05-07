#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, common::PhoneNumber callingNumber);

    void handleAccept() override;
    void handleReject() override;
    void handleDial(common::PhoneNumber to) override;

    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;
    void handleTimeout() override;
private:
    common::PhoneNumber callingNumber;
};

}
