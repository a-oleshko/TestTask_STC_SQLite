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
    BrokerSqliteClass brokerSqlite;
    brokerSqlite.moveToThread(&threadSqlBroker);
    threadSqlBroker.start(QThread::LowestPriority);

    //xml broker
    QThread threadXmlBroker;
    BrokerXmlClass brokerXml;
    brokerXml.moveToThread(&threadXmlBroker);
    threadXmlBroker.start(QThread::LowPriority);

    Model_EditorTable dataModel{"none"};
    QObject::connect(&brokerXml, SIGNAL(signalSerializedData(const QString &)),
                     &dataModel, SLOT(slotImportItem(const QString &)));

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
