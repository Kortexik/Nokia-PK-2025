#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectingState : public BaseState
{
public:
    ConnectingState(Context& context);
    ConnectingState(Context& context, common::BtsId btsId);


    void handleAccept() override {}
    void handleReject() override {}
    void handleDial(common::PhoneNumber to) override {}

    void handleTimeout() override;
    void handleAttachAccept() final;
    void handleAttachReject() final;
};

}
