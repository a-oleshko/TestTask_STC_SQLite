#ifndef BROKER_SQLITE_H
#define BROKER_SQLITE_H

#include <QObject>

/*Lifecycle
 * launch broker
 * check for DB file
 * if no file-> create -> wait for update from model
 * if exist -> read, parse, update model-> wait for update from model
 */

class BrokerSqliteClass : public QObject
{
    Q_OBJECT
public:
    BrokerSqliteClass();



};

#endif // BROKER_SQLITE_H
