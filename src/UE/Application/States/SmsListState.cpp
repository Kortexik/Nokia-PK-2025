#include "SmsListState.hpp"
#include "ConnectedState.hpp"

namespace ue
{
    SmsListState::SmsListState(Context &context) : ConnectedState(context)
    {
        context.user.displaySmsList(context.smsDb.getAllSms());
    }

    void SmsListState::handleReject()
    {
        logger.logInfo("Back to ConnectedState from SmsListState");
        context.setState<ConnectedState>();
    }
}