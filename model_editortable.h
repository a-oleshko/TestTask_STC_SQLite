#ifndef MODEL_EDITORTABLE_H
#define MODEL_EDITORTABLE_H

/*Interfaces
 * setData - SQLite
 * importData - XML
 *
 */

#include <QAbstractTableModel>

#include "itemclass.h"

class ModelTableEditors : public QAbstractTableModel
{
    Q_OBJECT

private:
    const size_t NUM_OF_COLUMNS{6};
    QString m_dbName{"stz.db"};

    QList<EditorItemClass> m_dataContainer;

    int m_editableRow{-1}; //-1 no row can be edited

public:
    explicit ModelTableEditors(const QString &dbName);

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
    void slotExportItem(int rowNumber, const QString &refFilename);
    void slotEditItem(int rowNumber);
    void slotClearModel();

    void slotDumpToSqlite();

signals:
    //to xml_broker
    void signalExportItem(const QString &refSerializedItem, const QString &refFilename);
    //to sqlite
    void signalExportItemSqlite(const QString &refSerializedItem);
    void signalDeleteItemSqlite(const QString &refEditorName);
    void signalUpdateItemSqlite(const QString &refSerializedItem);
    void signalClearTableSqlite();
};

#endif // MODEL_EDITORTABLE_H
