#pragma once

#include "ConnectedState.hpp"
#include "Sms.hpp"
#include "SmsListState.hpp"

namespace ue
{
class ViewSmsContentState : public ConnectedState
{
 public:
    ViewSmsContentState(Context &context, Sms &sms);
    void handleReject() override;
};
} // namespace ue