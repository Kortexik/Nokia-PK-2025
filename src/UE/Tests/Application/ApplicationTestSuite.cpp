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

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

    //NOT CONNECTED
    struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {
    };



    //CONNECTING
    struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite {
        ApplicationConnectingTestSuite()
        {
            changeStateToConnecting();
        }
        void changeStateToConnecting()
        {
            EXPECT_CALL(userPortMock, showConnecting());
            EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
            EXPECT_CALL(timerPortMock, startTimer(std::chrono::milliseconds(500)));

            objectUnderTest.handleSib(BTS_ID);
        }

    };

    TEST_F(ApplicationConnectingTestSuite, shallSendAttachRequest)
    {
        // constructor change the state
        SUCCEED();
    }

    TEST_F(ApplicationConnectingTestSuite, shallHandleDisconnectWhileConnecting)
    {
        EXPECT_CALL(userPortMock, showNotConnected());
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        objectUnderTest.handleDisconnected();
    }



    struct ApplicationConnectedTestSuite : ApplicationTestSuite {
        ApplicationConnectedTestSuite();
    };





}
