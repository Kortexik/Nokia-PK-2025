#include "QtSelectionListMode.hpp"
#include "QtUeGui.hpp"

#include <QListWidgetItem>

namespace ue
{

QtSelectionListMode::QtSelectionListMode(QtPhoneNumberEdit &phoneNumberTextEdit,
                                         QtStackedWidget &stackedWidget)
    : QtUeModeWidget(phoneNumberTextEdit, stackedWidget)
{
    constructGui();
    connectSignals();
}

QFont QtSelectionListMode::getItemFont()
{
    QFont font = QFont ("Courier");
    font.setStyleHint (QFont::Monospace);
    font.setPointSize (20);
    font.setFixedPitch (true);
    font.setBold(true);

    return font;
}

void QtSelectionListMode::addSelectionListItemSlot(QString label, QString tooltip)
{
    QListWidgetItem* newItem = new QListWidgetItem(label, &listWidget);
    newItem->setFont(getItemFont());
    if (not tooltip.isEmpty())
    {
        newItem->setToolTip(tooltip);
    }
}

void QtSelectionListMode::clearSelectionListSlot()
{
    listWidget.clear();
}

void QtSelectionListMode::constructGui()
{
    addChildWidget(&listWidget);

    listWidget.setStyleSheet( "QListWidget::item { border-bottom: 1px solid black; }");
    listWidget.setStyleSheet( "QListWidget::item:selected { border-color: darkblue; background: rgba(100, 100, 100, 200);}" );

    listWidget.show();
}

void QtSelectionListMode::connectSignals()
{
    connect(&listWidget, &QListWidget::doubleClicked, [this](const QModelIndex&){ emit itemDoubleClicked();});
    connect(this,SIGNAL(addSelectionListItemSignal(QString, QString)),this,SLOT(addSelectionListItemSlot(QString, QString)));
    connect(this,SIGNAL(clearSelectionListSignal()),this,SLOT(clearSelectionListSlot()));
}

void QtSelectionListMode::activateSlot()
{
    activateWithPhoneNumberEditDisabled();
}

IUeGui::IListViewMode::OptionalSelection QtSelectionListMode::  getCurrentItemIndex() const
{
    auto currentItem = listWidget.currentItem();
    if (currentItem)
    {
        return std::make_pair(true, listWidget.row(currentItem));
    }
    return std::make_pair(false, 0);
}

void QtSelectionListMode::addSelectionListItem(const std::string &label, const std::string &tooltip)
{
    emit addSelectionListItemSignal(QString::fromStdString(label), QString::fromStdString(tooltip));
}

void QtSelectionListMode::clearSelectionList()
{
    emit clearSelectionListSignal();
}

}
