#ifndef BROKER_XML_H
#define BROKER_XML_H

#include <QtXml>
#include <QFileDialog>

#ifdef QT_DEBUG
#include <QtDebug>
#endif

#include "itemclass.h"


//progress and report
//void slotReceiveReport(const QString &refImportReport);

class BrokerXmlClass : public QObject
{
    Q_OBJECT
private:
    QString m_selectedDir;
    QStringList m_listOfFiles;
    QString m_lastErrorMessage;

public:
    BrokerXmlClass();

protected:
    //main process
    void prepareFileList();
    void processFiles();
    QString getSerializedDataFromXmlFile(const QString &refFilename) const;

    //export


    //report
    void processErrorMessage(const QString &refFilename, const QString &refErrorDetails);

public slots:
    void slotSelectAndProcessDir(const QString &refDirpath);
    void slotExportEditorItem(const QString &refSerializedData);

signals:
    //model signals
    void signalSerializedData(const QString &refData) const;

    //import ui signals
    void signalImportStarted() const;
    void signalProgressUpperLimit(size_t filesCount) const;
    void signalCurrentProgress(size_t currentFileNo) const;
    void signalCurrentFilename(const QString &refFilename) const;
    void signalSendReport(const QString &refReportText) const;
};

#endif // BROKER_XML_H
