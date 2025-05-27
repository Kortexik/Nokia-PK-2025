#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class SmsListState : public ue::ConnectedState
{
 public:
    SmsListState(Context &context);
    void handleReject() override;
    void handleMenuSelection(unsigned int index) override;
    void handleSmsReceived(common::PhoneNumber from, const std::string &message) override;
};

} // namespace ue