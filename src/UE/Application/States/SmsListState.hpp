#pragma once

#include "ConnectedState.hpp"

namespace ue
{

class SmsListState : public ue::ConnectedState
{
public:
    SmsListState(Context &context);
    void handleReject() override;
};

}