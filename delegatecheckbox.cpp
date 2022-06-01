#include "delegatecheckbox.h"
#include <QCheckBox>
#include <QApplication>
#include <QAbstractItemView>
#include <QPainter>

DelegateCheckBox::DelegateCheckBox(QObject *parent) : QStyledItemDelegate (parent)
{

}

void DelegateCheckBox::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool data = index.model()->data(index, Qt::DisplayRole).toBool();

    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator,
                                                                &checkboxstyle);

    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/2 - checkbox_rect.width()/2);
    if(data)
        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);

    //TODO:implement coloring when selected
}

QWidget *DelegateCheckBox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QCheckBox *editor = new QCheckBox(parent);
    return editor;
}

void DelegateCheckBox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *cb = qobject_cast<QCheckBox *>(editor);
    cb->setChecked(index.data().toBool());
}

void DelegateCheckBox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *cb = static_cast<QCheckBox *>(editor);
    QString value = (cb->checkState()==Qt::Checked)? "true" : "false";
    model->setData(index, value, Qt::EditRole);
}

void DelegateCheckBox::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    QStyleOptionButton checkboxstyle;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator,
                                                                &checkboxstyle);
    checkboxstyle.rect = option.rect;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/2 - checkbox_rect.width()/2);
    editor->setGeometry(checkboxstyle.rect);
}
