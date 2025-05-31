#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IMockSmsDb.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
 protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{40};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<IMockSmsDb> smsDbMock;

    std::unique_ptr<Application> objectUnderTest;

    void SetUp() override
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest = std::make_unique<Application>(
    PHONE_NUMBER, loggerMock, btsPortMock, userPortMock, timerPortMock, smsDbMock
    );


    }
};

// NOT CONNECTED

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void sendAttachRequest()
    {
        using namespace std::chrono_literals;
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());

        objectUnderTest->handleSib(BTS_ID);
    }
};

TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestOnSib)
{
    sendAttachRequest();
}

// CONNECTING

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    void SetUp() override
    {
        ApplicationNotConnectedTestSuite::SetUp();
        sendAttachRequest();
    }
};

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());

    objectUnderTest->handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallConnectOnAttachAccept)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(userPortMock, showNewSms(_)).Times(AtLeast(1));
    EXPECT_CALL(smsDbMock, getUnreadCount());
    objectUnderTest->handleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());

    objectUnderTest->handleTimeout();
}

// CONNECTED

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    const common::PhoneNumber REMOTE_PHONE_NUMBER{113};
    void SetUp() override
    {
        ApplicationConnectingTestSuite::SetUp();

        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showConnected()).Times(AtLeast(1));
        EXPECT_CALL(userPortMock, showNewSms(_)).Times(AtLeast(1));
        EXPECT_CALL(smsDbMock, getUnreadCount()).Times(AtLeast(1));

        objectUnderTest->handleAttachAccept();
    }
};

TEST_F(ApplicationConnectedTestSuite, shallShowNotConnectedOnDisconnected)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest->handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallShowIncomingCallRequest)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showCallRequest(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest->handleCallRequest(REMOTE_PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallShowImCalling)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showDialing());
    EXPECT_CALL(btsPortMock, sendCallRequest(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, waitingForCallRespond(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest->handleDial(REMOTE_PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallDropCallOnCallRequestReject)
{

    using namespace std::chrono_literals;

    EXPECT_CALL(userPortMock, showDialing());
    EXPECT_CALL(btsPortMock, sendCallRequest(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, waitingForCallRespond(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest->handleDial(REMOTE_PHONE_NUMBER);

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDrop(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest->handleReject();
}
TEST_F(ApplicationConnectedTestSuite, shallDropCallOnCallRequestTimeout)
{

    using namespace std::chrono_literals;

    EXPECT_CALL(userPortMock, showDialing());
    EXPECT_CALL(btsPortMock, sendCallRequest(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, waitingForCallRespond(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest->handleDial(REMOTE_PHONE_NUMBER);

    EXPECT_CALL(btsPortMock, sendCallDrop(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showCallDropped(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest->handleTimeout();
}
TEST_F(ApplicationConnectedTestSuite, shallAcceptIncomingCall)
{
    using namespace std::chrono_literals;

    EXPECT_CALL(userPortMock, showDialing());
    EXPECT_CALL(btsPortMock, sendCallRequest(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, waitingForCallRespond(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest->handleDial(REMOTE_PHONE_NUMBER);

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccepted(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(userPortMock, setCallMode(REMOTE_PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest->handleAccept();
}

//SMS
struct ApplicationSmsTestSuite : ApplicationConnectedTestSuite
{
    void SetUp() override
    {
        EXPECT_CALL(smsDbMock, getUnreadCount()).WillRepeatedly(Return(0));
        ApplicationConnectedTestSuite::SetUp();
    }

};

TEST_F(ApplicationSmsTestSuite, shallChangeStateToSmsCompose)
{
    EXPECT_CALL(userPortMock, displaySmsCompose());
    objectUnderTest->handleMenuSelection(0);
}

TEST_F(ApplicationSmsTestSuite, shallSendSmsToUe)
{
    EXPECT_CALL(userPortMock, displaySmsCompose());
    objectUnderTest->handleMenuSelection(0);

    EXPECT_CALL(userPortMock, getSmsComposeData())
        .WillOnce(Return(Sms{PHONE_NUMBER, "Test message", Sms::Status::SENT}));

    EXPECT_CALL(smsDbMock, addSentSms(PHONE_NUMBER, "Test message"));
    EXPECT_CALL(btsPortMock, sendSmsMessage(PHONE_NUMBER, "Test message"));
    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(userPortMock, showNewSms(_));

    objectUnderTest->handleAccept();
}


TEST_F(ApplicationSmsTestSuite, shallCloseSmsComposeState)
{
    EXPECT_CALL(userPortMock, displaySmsCompose());
    objectUnderTest->handleMenuSelection(0);

    EXPECT_CALL(userPortMock, showConnected());
    EXPECT_CALL(userPortMock, showNewSms(_));
    objectUnderTest->handleReject();
}

TEST_F(ApplicationSmsTestSuite, shallDisplaySmsListOnEntryToSmsListState)
{
    static std::vector<Sms> emptySmsList{};

    EXPECT_CALL(smsDbMock, getAllSms())
        .WillOnce(ReturnRef(emptySmsList));
    EXPECT_CALL(smsDbMock, getUnreadCount())
        .WillRepeatedly(Return(0));
    EXPECT_CALL(userPortMock, displaySmsList(_));

    objectUnderTest->handleMenuSelection(1);
}

TEST_F(ApplicationSmsTestSuite, shallReturnToConnectedStateOnRejectInSmsListState)
{
    std::vector<Sms> messages = {};
    EXPECT_CALL(smsDbMock, getAllSms()).WillRepeatedly(ReturnRef(messages));
    EXPECT_CALL(smsDbMock, getUnreadCount()).WillRepeatedly(Return(0));
    EXPECT_CALL(userPortMock, displaySmsList(_));
    EXPECT_CALL(userPortMock, showNewSms(false))
        .Times(::testing::AtLeast(1));

    objectUnderTest->handleMenuSelection(1);
}

TEST_F(ApplicationSmsTestSuite, shallHandleSmsReceiveInSmsListState)
{
    std::vector<Sms> messages = {};

    EXPECT_CALL(smsDbMock, getAllSms())
        .WillRepeatedly(ReturnRef(messages));

    EXPECT_CALL(smsDbMock, getUnreadCount())
        .WillRepeatedly(Return(1));

    EXPECT_CALL(userPortMock, displaySmsList(_))
        .Times(::testing::AtLeast(1));

    EXPECT_CALL(userPortMock, showNewSms(true))
        .Times(::testing::AtLeast(1));

    objectUnderTest->handleMenuSelection(1);

    EXPECT_CALL(smsDbMock, addReceivedSms(PHONE_NUMBER, "Hello world"))
        .WillOnce(Return(1));

    objectUnderTest->handleSmsReceived(PHONE_NUMBER, "Hello world");
}

TEST_F(ApplicationSmsTestSuite, shallDisplaySmsListOnRejectFromViewSmsContentStateReplacement)
{
    static std::vector<Sms> emptySmsList{};

    EXPECT_CALL(smsDbMock, getAllSms())
        .WillRepeatedly(ReturnRef(emptySmsList));

    EXPECT_CALL(smsDbMock, getUnreadCount())
        .WillRepeatedly(Return(0));

    EXPECT_CALL(userPortMock, displaySmsList(::testing::_));

    EXPECT_CALL(userPortMock, showNewSms(::testing::_)).Times(::testing::AtLeast(1));

    objectUnderTest->handleMenuSelection(1);
    objectUnderTest->handleReject();
}


    //Talking
struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    void SetUp() override
    {

        ApplicationConnectedTestSuite::SetUp();
        changeStateToTalking();
    }
    void changeStateToTalking()
    {
        using namespace std::chrono_literals;

        EXPECT_CALL(userPortMock, showDialing());
        EXPECT_CALL(btsPortMock, sendCallRequest(PHONE_NUMBER));
        EXPECT_CALL(userPortMock, waitingForCallRespond(PHONE_NUMBER));
        EXPECT_CALL(timerPortMock, startTimer(30000ms));
        objectUnderTest->handleDial(PHONE_NUMBER);

        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(btsPortMock, sendCallAccepted(PHONE_NUMBER));
        EXPECT_CALL(userPortMock, setCallMode(PHONE_NUMBER));
        EXPECT_CALL(timerPortMock, startTimer(120000ms));
        objectUnderTest->handleAccept();
    }
};
TEST_F(ApplicationTalkingTestSuite, shallHandleDropCallBySelf)
{
    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showCallDropped(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());

    objectUnderTest->handleReject();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleDropCallByRemote)
{
    using namespace std::chrono_literals;

    EXPECT_CALL(userPortMock, showCallDroppedAfterTalk(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest->handleCallDropped(PHONE_NUMBER);
}

TEST_F(ApplicationTalkingTestSuite, shallRestartTimerBySendMessage)
{
    using namespace std::chrono_literals;
    std::string test_f = "test_f";
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallTalk(PHONE_NUMBER, test_f));
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest->handleSendCallTalk(PHONE_NUMBER, test_f);
}
TEST_F(ApplicationTalkingTestSuite, shallRestartTimerByReceiveMessage)
{
    using namespace std::chrono_literals;
    std::string test_f = "test_f";
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, newCallMessage(test_f));
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest->handleReceivedCallTalk(test_f);
}

} // namespace ue
