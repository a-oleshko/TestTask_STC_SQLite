#include "model_editortable.h"

ModelTableEditors::ModelTableEditors(const QString &dbName)
{
    if(!dbName.isEmpty())
    {
        m_dbName = dbName;
    }
}

int ModelTableEditors::rowCount(const QModelIndex &parent) const
{
    return m_dataContainer.size();
}

int ModelTableEditors::columnCount(const QModelIndex &parent) const
{
    return NUM_OF_COLUMNS;
}

QVariant ModelTableEditors::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    int row = index.row();
    if(role == Qt::DisplayRole && row<m_dataContainer.size())
    {
        switch(col)
        {
        case 0:
            return m_dataContainer.at(row).m_textEditor;
        case 1:
            return m_dataContainer.at(row).m_fileFormats;
        case 2:
            return m_dataContainer.at(row).m_encoding;
        case 3:
            return m_dataContainer.at(row).m_hasIntellisense;
        case 4:
            return m_dataContainer.at(row).m_hasPlugins;
        case 5:
            return m_dataContainer.at(row).m_canCompile;
        }
    }
    return QVariant();
}

QVariant ModelTableEditors::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Editor");
        case 1:
            return QString("Formats");
        case 2:
            return QString("Encoding");
        case 3:
            return QString("Intellisense");
        case 4:
            return QString("Has plugins");
        case 5:
            return QString("Can compile");
        }
    }
    return QVariant();
}

bool ModelTableEditors::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && index.row() == m_editableRow)
    {
        if (!checkIndex(index))
        {
            return false;
        }
        if(value.toString().isEmpty() || value.toString().contains(":"))
        {
            return false;
        }
        switch(index.column())
        {
        case 0:
            (m_dataContainer.data()+index.row())->m_textEditor = value.toString();
            break;
        case 1:
            (m_dataContainer.data()+index.row())->m_fileFormats = value.toString();
            break;
        case 2:
            (m_dataContainer.data()+index.row())->m_encoding = value.toString();
            break;
        case 3:
            (m_dataContainer.data()+index.row())->m_hasIntellisense = value.toBool();
            break;
        case 4:
            (m_dataContainer.data()+index.row())->m_hasPlugins = value.toBool();
            break;
        case 5:
            (m_dataContainer.data()+index.row())->m_canCompile = value.toBool();
            break;
        }
        emit signalUpdateItemSqlite(m_dataContainer.at(index.row()).toString());
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ModelTableEditors::flags(const QModelIndex &index) const
{    
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ModelTableEditors::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);

    endInsertRows();
    return true;
}

bool ModelTableEditors::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    endRemoveRows();
    return true;
}


void ModelTableEditors::loadData()
{

}

void ModelTableEditors::slotImportItem(const QString &refSerializedItem)
{
    try
    {
        EditorItemClass newItem(refSerializedItem);

        if(!m_dataContainer.contains(newItem))
        {
            insertRows(m_dataContainer.size(), 1);
            m_dataContainer<<newItem;
            if(sender()->objectName() != "SQLITE")
            {
                emit signalExportItemSqlite(newItem.toString());
            }
        }
    }
    catch(int errNo)
    {
        return;
    }
}

void ModelTableEditors::slotDeleteItem(int rowNumber)
{
    if(rowNumber>-1 && rowNumber<m_dataContainer.size())
    {
        removeRows(rowNumber, 1);
        emit signalDeleteItemSqlite(m_dataContainer.at(rowNumber).m_textEditor);
        m_dataContainer.remove(rowNumber,1);
    }
}

void ModelTableEditors::slotExportItem(int rowNumber, const QString &refFilename)
{
    if(rowNumber>-1 && rowNumber<m_dataContainer.size())
    {
        emit signalExportItem(m_dataContainer.at(rowNumber).toString(), refFilename);
    }
}

void ModelTableEditors::slotEditItem(int rowNumber)
{
    m_editableRow = rowNumber;
}

void ModelTableEditors::slotClearModel()
{
    removeRows(0, m_dataContainer.size());
    m_dataContainer.clear();
    emit signalClearTableSqlite();
}

void ModelTableEditors::slotDumpToSqlite()
{

}
