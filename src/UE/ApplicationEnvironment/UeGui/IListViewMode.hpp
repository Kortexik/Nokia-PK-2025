#pragma once

#include "IUeGui.hpp"
#include <utility>

namespace ue
{

class IUeGui::IListViewMode
{
 public:
    using Selection = unsigned;
    using OptionalSelection = std::pair<bool, Selection>;

    virtual ~IListViewMode() = default;

    virtual OptionalSelection getCurrentItemIndex() const = 0;
    virtual void addSelectionListItem(const std::string &label, const std::string &tooltip) = 0;
    virtual void clearSelectionList() = 0;
};

} // namespace ue
