#pragma once

#include "ConnectedState.hpp"

namespace ue
{
class SmsComposeState : public ConnectedState
{
 public:
    SmsComposeState(Context &context);
    void handleAccept() override;
    void handleReject() override;
};
} // namespace ue
