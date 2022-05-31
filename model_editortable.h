#ifndef MODEL_EDITORTABLE_H
#define MODEL_EDITORTABLE_H

/*Interfaces
 * setData - SQLite
 * importData - XML
 *
 */

#include <QAbstractTableModel>

#include "itemclass.h"

class Model_EditorTable : public QAbstractTableModel
{
    Q_OBJECT

private:
    const size_t NUM_OF_COLUMNS{6};
    QString m_dbName{"stz.db"};

    QList<EditorItemClass> m_dataContainer;

public:
    explicit Model_EditorTable(const QString &dbName);

    //CRUD item
    //add idem
    //delete item
    //export item
    //update item

    //upstream to SQLite file

    //populate table on init

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    void loadData();
public slots:
    void slotImportItem(const QString &refSerializedItem);
    void slotDeleteItem(int rowNumber);
    void slotExportItem(int rowNumber);
    void slotClearModel();

    void slotDumpToSqlite();

signals:
    void signalExportItem(const QString &refSerializedItem) const; //to xml_broker
};

#endif // MODEL_EDITORTABLE_H
