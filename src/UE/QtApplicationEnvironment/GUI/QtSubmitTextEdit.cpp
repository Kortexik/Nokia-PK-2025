#include "QtSubmitTextEdit.hpp"

namespace ue
{

void QtSubmitTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        emit submitted();
    }
    else
    {
        QTextEdit::keyPressEvent(event);
    }
}

} // namespace ue
