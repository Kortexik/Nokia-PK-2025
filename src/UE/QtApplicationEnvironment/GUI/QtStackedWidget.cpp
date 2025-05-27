#include "QtStackedWidget.hpp"

namespace ue
{

QtStackedWidget::QtStackedWidget()
{
}

QtStackedWidget::~QtStackedWidget()
{
}

int QtStackedWidget::addModeWidget(QWidget *widget)
{
    auto newIndex = count();
    insertWidget(newIndex, widget);
    return newIndex;
}

} // namespace ue
