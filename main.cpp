#include "OnlyWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "model_editortable.h"
#include "broker_xml.h"
#include "broker_sqlite.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //sql-broker
    QThread threadSqlBroker;
    BrokerSqliteClass brokerSqlite{"dbfile.sqlite"};
    brokerSqlite.moveToThread(&threadSqlBroker);
    QObject::connect(&threadSqlBroker, SIGNAL(started()),
                      &brokerSqlite, SLOT(slotInitDB()));

    //xml broker
    QThread threadXmlBroker;
    BrokerXmlClass brokerXml;
    brokerXml.moveToThread(&threadXmlBroker);

    ModelTableEditors dataModel{"none"};
    QObject::connect(&brokerXml, SIGNAL(signalSerializedData(const QString &)),
                     &dataModel, SLOT(slotImportItem(const QString &)));

    QObject::connect(&brokerSqlite, SIGNAL(signalSerializedData(const QString &)),
                     &dataModel, SLOT(slotImportItem(const QString &)));
    QObject::connect(&dataModel, SIGNAL(signalExportItemSqlite(const QString &)),
                     &brokerSqlite, SLOT(slotCreateSqliteRecord(const QString &)));
    QObject::connect(&dataModel, SIGNAL(signalDeleteItemSqlite(const QString &)),
                     &brokerSqlite, SLOT(slotDeleteSqliteRecord(const QString &)));
    QObject::connect(&dataModel, SIGNAL(signalUpdateItemSqlite(const QString &)),
                     &brokerSqlite, SLOT(slotUpdateSqliteRecord(const QString &)));
    QObject::connect(&dataModel, SIGNAL(signalClearTableSqlite()),
                     &brokerSqlite, SLOT(slotClearSqliteRecords()));




    //connect signals and slots model and sql

    threadSqlBroker.start(QThread::LowestPriority);
    threadXmlBroker.start(QThread::LowPriority);

    OnlyWindow mainWindow(&dataModel, &brokerXml, &brokerSqlite);

    QObject::connect(&mainWindow, SIGNAL(destroyed()),
                     &threadSqlBroker, SLOT(terminate()));
    QObject::connect(&mainWindow, SIGNAL(destroyed()),
                     &threadXmlBroker, SLOT(terminate()));

    mainWindow.show();
    return app.exec();
}


//translator

//QTranslator translator;
//const QStringList uiLanguages = QLocale::system().uiLanguages();
//for (const QString &locale : uiLanguages) {
//    const QString baseName = "TestTask_STC_SQLite_" + QLocale(locale).name();
//    if (translator.load(":/i18n/" + baseName)) {
//        a.installTranslator(&translator);
//        break;
//    }
//}
