#ifndef BROKER_SQLITE_H
#define BROKER_SQLITE_H

#include <QObject>
#include <QtSql>

/*Lifecycle
 * launch broker
 * check for DB file
 * if no file-> create -> wait for update from model
 * if exists -> read, parse, update model-> wait for update from model
 */

//NOTE: texteditor must be unique - my assumption

class BrokerSqliteClass : public QObject
{
    Q_OBJECT
private:
    QString m_dbFilename;
    QSqlDatabase *m_objDatabase{nullptr};

    QStringList m_cachedListOfData;//NOTE: needed or not
public:
    BrokerSqliteClass(const QString &refDBFilename, QObject *parent = nullptr);
    ~BrokerSqliteClass();

public slots:
    void slotInitDB();
    void slotCreateSqliteRecord(const QString &refSerializedString);
    void slotDeleteSqliteRecord(const QString &refSerializedString);
    void slotUpdateSqliteRecord(const QString &refSerializedString);
    void slotClearSqliteRecords();

protected:
    void init();
    void processExistingRecords();

    //create record
    void createRecord(const QString &refSerializedString);
    QString readRecord();
    void updateRecord(const QString &refSerializedString);
    void deleteRecord(const QString &refSerializedString);
    QStringList listRecords();

    QString stringToRequest(const QString &refSerializedData);
    QString requestToString(const QString &refSerializedData);
signals:
    void signalSerializedData(const QString &refSerializedString);
};

#endif // BROKER_SQLITE_H
