#ifndef DELEGATECHECKBOX_H
#define DELEGATECHECKBOX_H

#include <QStyledItemDelegate>


class DelegateCheckBox : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DelegateCheckBox(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DELEGATECHECKBOX_H
