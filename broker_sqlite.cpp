#include "broker_sqlite.h"

BrokerSqliteClass::BrokerSqliteClass(const QString &refDBFilename, QObject *parent)
    : QObject(parent)
{
    setObjectName("SQLITE");
    if(refDBFilename.isEmpty())
    {
        m_dbFilename = "dbfile.sqlite";
    }
    else
    {
        m_dbFilename = refDBFilename;
    }
}

BrokerSqliteClass::~BrokerSqliteClass()
{
    m_objDatabase->close();
    delete m_objDatabase;
}

void BrokerSqliteClass::slotInitDB()
{
    init();
}

void BrokerSqliteClass::slotCreateSqliteRecord(const QString &refSerializedString)
{
    QStringList itemData = refSerializedString.split(":");
    QString sqliteQueryTemplate = QString("INSERT INTO editors ").
            append("(texteditor, fileformats, encoding, hasintellisense, hasplugins, cancompile)").
            append(" VALUES('%1', '%2', '%3', '%4', '%5', '%6');");

    QString sqliteQuery = sqliteQueryTemplate.arg(itemData.at(0)).arg(itemData.at(1)).
            arg(itemData.at(2)).arg((itemData.at(3)=="true")?"1":"0").
            arg((itemData.at(4)=="true")?"1":"0").arg((itemData.at(5)=="true")?"1":"0");

    QSqlQuery sqlCreateItemRecord;
    if(!sqlCreateItemRecord.exec(sqliteQuery))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"unable to create new record"<<sqlCreateItemRecord.lastError();
    }
}

void BrokerSqliteClass::slotDeleteSqliteRecord(const QString &refEditorName)
{
    QString sqliteQueryTemplate = QString("DELETE FROM  editors WHERE texteditor ='%1';");
    QString sqliteQuery = sqliteQueryTemplate.arg(refEditorName);

    QSqlQuery sqlDeleteItemRecord;
    if(!sqlDeleteItemRecord.exec(sqliteQuery))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"unable to delete record"<<sqlDeleteItemRecord.lastError();
    }
}

void BrokerSqliteClass::slotUpdateSqliteRecord(const QString &refSerializedString)
{
    qDebug()<<__PRETTY_FUNCTION__<<refSerializedString;
    QStringList itemData = refSerializedString.split(":");
    QString sqliteQueryTemplate = "UPDATE editors SET fileformats = '%2', encoding = '%3', "
                            "hasintellisense = '%4', hasplugins = '%5', cancompile = '%6' "
                            "WHERE texteditor = '%1';";

    QString sqliteQuery = sqliteQueryTemplate.arg(itemData.at(0)).arg(itemData.at(1)).
            arg(itemData.at(2)).arg((itemData.at(3)=="true")?"1":"0").
            arg((itemData.at(4)=="true")?"1":"0").arg((itemData.at(5)=="true")?"1":"0");

    qDebug()<<__PRETTY_FUNCTION__<<sqliteQuery;

    QSqlQuery sqlUpdateItemRecord;
    if(!sqlUpdateItemRecord.exec(sqliteQuery))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"unable to update existing record"<<sqlUpdateItemRecord.lastError();
    }
}

void BrokerSqliteClass::slotClearSqliteRecords()
{
    QString sqliteQuery= "DELETE FROM editors";
    QSqlQuery sqlTruncateRecords;
    if(!sqlTruncateRecords.exec(sqliteQuery))
    {
        qDebug()<<__PRETTY_FUNCTION__<<"unable to clear table"<<sqlTruncateRecords.lastError();
    }

}

void BrokerSqliteClass::init()
{
    m_objDatabase = new QSqlDatabase();
    *m_objDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_objDatabase->setDatabaseName(m_dbFilename);

    if(!QFile::exists(m_dbFilename))
    {
        m_objDatabase->open();
        QSqlQuery query;
        query.exec("create table editors "
                   "(texteditor varchar(20), "
                   "fileformats varchar(30), "
                   "encoding varchar(20), "
                   "hasintellisense integer,"
                   "hasplugins int,"
                   "cancompile int)");
    }
    else
    {
        m_objDatabase->open();
        processExistingRecords();
    }
}

void BrokerSqliteClass::processExistingRecords()
{
    for(auto &item : listRecords())
    {
        emit signalSerializedData(item);
    }
}

QStringList BrokerSqliteClass::listRecords()
{
    QStringList retListOfSerializedEditors;

    QSqlQuery sqlBoxIdQuery;
    if(!sqlBoxIdQuery.exec("SELECT * FROM editors;"))
    {
        retListOfSerializedEditors.clear();
    }
    else
    {
        QSqlRecord sqlRecord = sqlBoxIdQuery.record();
        while(sqlBoxIdQuery.next())
        {
            if(!retListOfSerializedEditors.contains(sqlBoxIdQuery.value(sqlRecord.indexOf("texteditor")).toString()))
            {
                QString testeditor = sqlBoxIdQuery.value(sqlRecord.indexOf("texteditor")).toString();
                QString fileformats = sqlBoxIdQuery.value(sqlRecord.indexOf("fileformats")).toString();
                QString encoding = sqlBoxIdQuery.value(sqlRecord.indexOf("encoding")).toString();
                QString hasintellisense = sqlBoxIdQuery.value(sqlRecord.indexOf("hasintellisense")).toBool()?"true":"false";
                QString hasplugins = sqlBoxIdQuery.value(sqlRecord.indexOf("hasplugins")).toBool()?"true":"false";
                QString cancompile = sqlBoxIdQuery.value(sqlRecord.indexOf("cancompile")).toBool()?"true":"false";
                QString serializedString;
                serializedString.append(testeditor).append(":").append(fileformats).append(":").
                        append(encoding).append(":").append(hasintellisense).append(":").
                        append(hasplugins).append(":").append(cancompile);
                retListOfSerializedEditors<<serializedString;
            }
        }
    }
    return retListOfSerializedEditors;
}

QString BrokerSqliteClass::stringToRequest(const QString &refSerializedData)
{

}

QString BrokerSqliteClass::requestToString(const QString &refSerializedData)
{

}

