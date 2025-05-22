#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
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

    std::unique_ptr<Application> objectUnderTest;

    void SetUp() override
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest = std::make_unique<Application>(PHONE_NUMBER, loggerMock, btsPortMock, userPortMock, timerPortMock
        );
    }
};

    //NOT CONNECTED

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

    //CONNECTING

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

    objectUnderTest->handleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());

    objectUnderTest->handleTimeout();
}

    //CONNECTED

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    const common::PhoneNumber REMOTE_PHONE_NUMBER{113};
    void SetUp() override
    {
        ApplicationConnectingTestSuite::SetUp();

        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showConnected());

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
    EXPECT_CALL(userPortMock,waitingForCallRespond(REMOTE_PHONE_NUMBER) );
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
    TEST_F(ApplicationConnectedTestSuite, shallAcceptIncomingCall) {
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


    //Talking

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    void SetUp() override
    {

        ApplicationConnectedTestSuite::SetUp();
        changeStateToTalking();
    }
    void changeStateToTalking() {
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
    EXPECT_CALL(btsPortMock, sendCallTalk(PHONE_NUMBER,test_f));
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest->handleSendCallTalk(PHONE_NUMBER,test_f);
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



}
