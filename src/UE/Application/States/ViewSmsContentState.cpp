#include "ViewSmsContentState.hpp"

namespace ue
{
    ViewSmsContentState::ViewSmsContentState(Context &context, Sms& sms) : ConnectedState(context)
    {
        logger.logInfo("Displaying SMS with attached number: " + common::to_string(sms.getPhoneNumber()));
        logger.logInfo("Status enum value: " + std::to_string(static_cast<int>(sms.getStatus())));
        context.user.displaySmsContent(sms);
    }

    void ViewSmsContentState::handleReject()
    {
        context.setState<SmsListState>();
    }
}