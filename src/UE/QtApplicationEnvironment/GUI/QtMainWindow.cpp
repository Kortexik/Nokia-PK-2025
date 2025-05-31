#include "QtMainWindow.hpp"

namespace ue
{

QtMainWindow::QtMainWindow()
{
    setCloseGuard(IUeGui::CloseGuard{});
}

void QtMainWindow::setCloseGuard(const IUeGui::CloseGuard &newCloseGuard)
{
    if (newCloseGuard)
    {
        closeGuard = newCloseGuard;
    }
    else
    {
        closeGuard = [] { return true; };
    }
}
bool QtMainWindow::closeAccepted()
{
    return closeGuard();
}

void QtMainWindow::closeEvent(QCloseEvent *event)
{
    if (closeAccepted())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

} // namespace ue
