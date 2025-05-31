#pragma once

#include "IUeGui.hpp"

namespace ue
{

class IUeGui::ITextMode
{
 public:
    virtual ~ITextMode() = default;
    virtual void setText(const std::string &text) = 0;
};

} // namespace ue
