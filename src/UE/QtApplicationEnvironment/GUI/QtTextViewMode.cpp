#include "QtTextViewMode.hpp"
#include "QtSmsComposeMode.hpp"

namespace ue
{

QtTextViewMode::QtTextViewMode(QtSmsComposeMode &baseMode) : baseMode(baseMode)
{
}

void QtTextViewMode::init()
{
}

void QtTextViewMode::activate()
{
    baseMode.activateForViewingSms();
}

void QtTextViewMode::setText(const std::string &value)
{
    baseMode.setSmsText(value);
}

} // namespace ue
