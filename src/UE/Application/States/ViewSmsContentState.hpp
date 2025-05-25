#pragma once

#include "ConnectedState.hpp"
#include "SmsListState.hpp"
#include "Sms.hpp"

namespace ue
{
class ViewSmsContentState : public ConnectedState
{
public:
    ViewSmsContentState(Context &context, Sms& sms);
    void handleReject() override;
};
}