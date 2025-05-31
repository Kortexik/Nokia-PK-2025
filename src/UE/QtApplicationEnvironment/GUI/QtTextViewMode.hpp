#pragma once

#include "UeGui/ITextMode.hpp"

namespace ue
{

class QtSmsComposeMode;

class QtTextViewMode : public IUeGui::ITextMode
{
 public:
    QtTextViewMode(QtSmsComposeMode &baseMode);
    void init();
    void activate();

    void setText(const std::string &) override;

 private:
    QtSmsComposeMode &baseMode;
};

} // namespace ue
