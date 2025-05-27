#pragma once

#include "IUeGui.hpp"

#include <QKeyEvent>
#include <QObject>
#include <QTextEdit>

#include "Logger/ILogger.hpp"

namespace ue
{

class QtSubmitTextEdit : public QTextEdit
{
    Q_OBJECT
 signals:
    void submitted();

 public:
    void keyPressEvent(QKeyEvent *event) override;
};

} // namespace ue
