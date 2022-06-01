#ifndef DELEGATEEDITABLEROW_H
#define DELEGATEEDITABLEROW_H

#include <QStyledItemDelegate>

#define FIRST_BOOL_COLUMN 3
class DelegateEditableRow: public QStyledItemDelegate
{
    Q_OBJECT
private:
    int m_editableRow{-1};
public:
    DelegateEditableRow(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void slotSetEditableRow(int rowNumber);
};

#endif // DELEGATEEDITABLEROW_H
