#include "delegateeditablerow.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QApplication>
#include <QAbstractItemView>
#include <QPainter>

DelegateEditableRow::DelegateEditableRow(QObject *parent): QStyledItemDelegate (parent)
{

}

void DelegateEditableRow::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(m_editableRow == index.row())
    {
        painter->fillRect(option.rect, Qt::green);
        painter->setOpacity(0.8);
    }

    if(index.column()>=FIRST_BOOL_COLUMN)
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

    }

    QStyledItemDelegate::paint(painter, option, index);
}

QWidget *DelegateEditableRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()>=FIRST_BOOL_COLUMN)
    {
        QCheckBox *editor = new QCheckBox(parent);
        if(m_editableRow != index.row())
        {
            editor->setEnabled(false);
        }
        return editor;
    }
    else
    {
        if(m_editableRow != index.row())
        {
            return Q_NULLPTR;
        }
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }
}

void DelegateEditableRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column()>=FIRST_BOOL_COLUMN)
    {
        QCheckBox *cb = qobject_cast<QCheckBox *>(editor);
        cb->setChecked(index.data().toBool());
    }
    else
    {
        QLineEdit *le = qobject_cast<QLineEdit *>(editor);
        le->setText(index.data().toString());
    }
}

void DelegateEditableRow::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column()>=FIRST_BOOL_COLUMN)
    {
        QCheckBox *cb = static_cast<QCheckBox *>(editor);
        QString value = (cb->checkState()==Qt::Checked)? "true" : "false";
        model->setData(index, value, Qt::EditRole);
    }
    else
    {
        QLineEdit *le = static_cast<QLineEdit *>(editor);
        QString value = le->text();
        model->setData(index, value, Qt::EditRole);
    }
}

void DelegateEditableRow::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()>=FIRST_BOOL_COLUMN)
    {
        QStyleOptionButton checkboxstyle;
        QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator,
                                                                    &checkboxstyle);
        checkboxstyle.rect = option.rect;
        checkboxstyle.rect.setLeft(option.rect.x() +
                                   option.rect.width()/2 - checkbox_rect.width()/2);
        editor->setGeometry(checkboxstyle.rect);
    }
    else
    {
        editor->setGeometry(option.rect);
    }
}

void DelegateEditableRow::slotSetEditableRow(int rowNumber)
{
    m_editableRow = rowNumber;
}

