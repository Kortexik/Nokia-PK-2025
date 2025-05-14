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

    Expectation expectShowNotConnected = EXPECT_CALL(userPortMock, showNotConnected());
    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

    //NOT CONNECTED
    struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {
        void sendAttachRequest();
    };
    void ApplicationNotConnectedTestSuite::sendAttachRequest()
    {
        using namespace std::chrono_literals;
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());
        objectUnderTest.handleSib(BTS_ID);
    }

    TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestOnSib)
    {
        sendAttachRequest();
    }

    //CONNECTING
    struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite {
        void SetUp() override
        {
            ApplicationTestSuite::SetUp();
            sendAttachRequest();
        }

    };
    TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
    {
        EXPECT_CALL(userPortMock, showNotConnected());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleAttachReject();
    }
    TEST_F(ApplicationConnectingTestSuite, shallConnectOnAttachAccept) {
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showConnected());
        objectUnderTest.handleAttachAccept();
    }
    TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
    {
        EXPECT_CALL(userPortMock, showNotConnected());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleTimeout();
    }



    struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite {
    };

    TEST_F(ApplicationConnectedTestSuite, shallShowNotConnectedOnDisconnected) {
        objectUnderTest.handleDisconnected();
    }






}
