#pragma once

#include "BaseState.hpp"

namespace ue
{

class NotConnectedState : public BaseState
{
 public:
    NotConnectedState(Context &context);
    void handleSib(common::BtsId) final;

    void handleAccept() override;
    void handleReject() override;
    void handleDial(common::PhoneNumber to) override;

    void handleAttachAccept() override;
    void handleAttachReject() override;
};
} // namespace ue
