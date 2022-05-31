#include "broker_xml.h"

BrokerXmlClass::BrokerXmlClass()
{
}

void BrokerXmlClass::prepareFileList()
{
    m_listOfFiles.clear();
    QStringList strlistFilename;
    QDir parsedPath(m_selectedDir);
    if(parsedPath.exists())
    {
        m_listOfFiles = parsedPath.entryList(QStringList()<<"*.xml", QDir::Files);
    }
    emit signalProgressUpperLimit(m_listOfFiles.size());
}

void BrokerXmlClass::processFiles()
{
    int fileCounter{1}, problemsCounter{0};
    m_lastErrorMessage.clear();
    for(auto &item : m_listOfFiles)
    {
        QString serializedResult;
        try
        {
            serializedResult.clear();
            serializedResult = getSerializedDataFromXmlFile(item);
            emit signalSerializedData(serializedResult);
        }
        catch(QString exception)//failed xml structure
        {
            problemsCounter++;
            processErrorMessage(item, exception);
        }
        catch(std::exception &exception)//io exception
        {
            problemsCounter++;
            processErrorMessage(item, exception.what());
        }
        emit signalCurrentProgress(fileCounter);
        fileCounter++;
        //_sleep(3000);//NOTE: just to check multithreading
    }
    if(problemsCounter>0)
    {
        emit signalSendReport(m_lastErrorMessage);
    }
    else
    {
        emit signalSendReport("Все файлы успешно импортированы.");
    }
}

QString BrokerXmlClass::getSerializedDataFromXmlFile(const QString &refFilename) const
{
    emit signalCurrentFilename(refFilename);
    QString filename = refFilename;
    filename.prepend("/").prepend(m_selectedDir);
    QFile xmlFile(filename);
    QDomDocument domdocData;
    QString textEditor, fileFormats, encoding, hasintellisens, hasplugins, cancompile;
    QString resultString;
    if(xmlFile.open(QIODevice::ReadOnly))
    {
        QString error;
        int line, column;
        if(domdocData.setContent(&xmlFile, &error, &line, &column))
        {
            QDomElement delementEditor = domdocData.documentElement();
            QDomNode dnodeSettingItem = delementEditor.firstChild();
            while(!dnodeSettingItem.isNull())
            {
                QDomElement delementConfigItem = dnodeSettingItem.toElement();
                if(delementConfigItem.tagName().toLower() == "texteditor")
                {
                    textEditor = delementConfigItem.text();
                }
                else if(delementConfigItem.tagName().toLower() == "fileformats")
                {
                    fileFormats = delementConfigItem.text();
                }
                else if(delementConfigItem.tagName().toLower() == "encoding")
                {
                    encoding = delementConfigItem.text();
                }
                else if(delementConfigItem.tagName().toLower() == "hasintellisense")
                {
                    hasintellisens = delementConfigItem.text();
                }
                else if(delementConfigItem.tagName().toLower() == "hasplugins")
                {
                    hasplugins = delementConfigItem.text();
                }
                else if(delementConfigItem.tagName().toLower() == "cancompile ")
                {
                    cancompile = delementConfigItem.text();
                }
                dnodeSettingItem = dnodeSettingItem.nextSibling();
            }
        }
        else
        {
            error.append(";").append(QString::number(line)).append(";").append(QString::number(column));
            throw error;
        }
        xmlFile.close();
    }
    else
    {
        throw "nofile"; //error openning file
    }

    resultString.append(textEditor).append(";").append(fileFormats).append(";").append(encoding).append(";").
            append(hasintellisens).append(";").append(hasplugins).append(";").append(cancompile);
    return resultString;
}

void BrokerXmlClass::processErrorMessage(const QString &refFilename, const QString &refErrorDetails)
{
    QString fileReport{"Файл: %1 \nПроблема: %2 \n\n"};
    m_lastErrorMessage.append(fileReport.arg(refFilename, refErrorDetails));
}

void BrokerXmlClass::slotSelectAndProcessDir(const QString &refDirpath)
{
    m_selectedDir = refDirpath;
    prepareFileList();
    processFiles();
}

void BrokerXmlClass::slotExportEditorItem(const QString &refSerializedData)
{
    qDebug()<<__PRETTY_FUNCTION__<<"try to export"<<refSerializedData;
}
