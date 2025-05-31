#pragma once

#include "ISmsDb.hpp"
#include <gmock/gmock.h>

namespace ue
{

class IMockSmsDb : public ISmsDb
{
public:
    IMockSmsDb();
    ~IMockSmsDb() override;

    MOCK_METHOD(std::size_t, addReceivedSms, (common::PhoneNumber, const std::string&), (override));
    MOCK_METHOD(std::size_t, addSentSms, (common::PhoneNumber, const std::string&), (override));
    MOCK_METHOD(std::vector<Sms>&, getAllSms, (), (override));
    MOCK_METHOD(std::size_t, getUnreadCount, (), (const, override));
};

}



