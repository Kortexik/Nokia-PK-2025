#pragma once

#include "IUeGui.hpp"
#include "UeGui/ICallMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/ITextMode.hpp"
#include <gmock/gmock.h>

namespace ue
{

struct IUeGuiMock : public IUeGui
{
    IUeGuiMock();
    ~IUeGuiMock() override;

    MOCK_METHOD(void, setCloseGuard, (CloseGuard closeGuard), (final));
    MOCK_METHOD(void, setAcceptCallback, (Callback), (final));
    MOCK_METHOD(void, setRejectCallback, (Callback), (final));
    MOCK_METHOD(void, setTitle, (const std::string &title), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showNewSms, (bool present), (final));
    MOCK_METHOD(void, showPeerUserNotAvailable, (common::PhoneNumber), (final));

    MOCK_METHOD(IListViewMode &, setListViewMode, (), (final));
    MOCK_METHOD(ISmsComposeMode &, setSmsComposeMode, (), (final));
    MOCK_METHOD(IDialMode &, setDialMode, (), (final));
    MOCK_METHOD(ICallMode &, setCallMode, (), (final));
    MOCK_METHOD(ITextMode &, setAlertMode, (), (final));
    MOCK_METHOD(ITextMode &, setViewTextMode, (), (final));
};

class IListViewModeMock : public IUeGui::IListViewMode
{
 public:
    IListViewModeMock();
    ~IListViewModeMock() override;

    MOCK_METHOD(OptionalSelection, getCurrentItemIndex, (), (const, final));
    MOCK_METHOD(void, addSelectionListItem, (const std::string &label, const std::string &tooltip), (final));
    MOCK_METHOD(void, clearSelectionList, (), (final));
};

class ITextModeMock : public IUeGui::ITextMode
{
 public:
    ITextModeMock();
    ~ITextModeMock() override;

    MOCK_METHOD(void, setText, (const std::string &text), (final));
};

class ISmsComposeModeMock : public IUeGui::ISmsComposeMode
{
 public:
    ISmsComposeModeMock();
    ~ISmsComposeModeMock() override;

    MOCK_METHOD(PhoneNumber, getPhoneNumber, (), (const, final));
    MOCK_METHOD(std::string, getSmsText, (), (const, final));
    MOCK_METHOD(void, clearSmsText, (), (final));
};

class ICallModeMock : public IUeGui::ICallMode
{
 public:
    ICallModeMock();
    ~ICallModeMock() override;

    MOCK_METHOD(void, clearIncomingText, (), (final));
    MOCK_METHOD(void, appendIncomingText, (const std::string &text), (final));
    MOCK_METHOD(void, clearOutgoingText, (), (final));
    MOCK_METHOD(std::string, getOutgoingText, (), (const, final));
};

class IDialModeMock : public IUeGui::IDialMode
{
 public:
    IDialModeMock();
    ~IDialModeMock() override;

    MOCK_METHOD(PhoneNumber, getPhoneNumber, (), (const, final));
};

} // namespace ue
