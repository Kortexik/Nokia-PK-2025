#pragma once

#include <QObject>
#include <QStackedWidget>
#include <QWidget>

#include "Logger/ILogger.hpp"

namespace ue
{

using ILogger = common::ILogger;

class QtStackedWidget : public QStackedWidget
{
    Q_OBJECT
 public:
    QtStackedWidget();
    ~QtStackedWidget();

    int addModeWidget(QWidget *widget);
    using QStackedWidget::setCurrentIndex;
};

} // namespace ue
