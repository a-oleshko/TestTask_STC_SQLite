#include "broker_xml.h"

BrokerXmlClass::BrokerXmlClass()
{
}

void BrokerXmlClass::prepareFileList()
{
    m_listOfFiles.clear();
    QDir parsedPath(m_selectedDir);
    if(parsedPath.exists())
    {
        m_listOfFiles = parsedPath.entryList(QStringList()<<"*.xml", QDir::Files);
    }
    emit signalProgressUpperLimit(m_listOfFiles.size());
}

void BrokerXmlClass::processFiles()
{
    int fileCounter{0}, problemsCounter{0};
    m_importReportMessage.clear();
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
        fileCounter++;
        emit signalCurrentProgress(fileCounter);
        //_sleep(3000);//NOTE: just to check multithreading
    }
    prepareReport(fileCounter, problemsCounter);
    emit signalSendReport(m_importReportMessage);
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

    resultString.append(textEditor).append(":").append(fileFormats).append(":").append(encoding).append(":").
            append(hasintellisens).append(":").append(hasplugins).append(":").append(cancompile);
    return resultString;
}

void BrokerXmlClass::prepareReport(int totalNumOfFiles, int problemCounter)
{
    if(problemCounter < 1)
    {
        QString fullSuccessString{"Успешно импортировано: %1 файл(-а/ов) из %1."};
        m_importReportMessage.append(fullSuccessString.arg(totalNumOfFiles));
    }
    else if(totalNumOfFiles == problemCounter)
    {
        //full fail
        QString fullFailString{"Импортировано с ошибками: %1 файл(-а/ов) из %1\n"
                               "Перечень файлов с ошибками:\n"};
        m_importReportMessage.prepend(fullFailString.arg(totalNumOfFiles));
    }
    else
    {
        QString partialSuccessString{"Всего файлов: %1.\n"
                                     "Из них успешно: %2.\n"
                                     "С ошибками: %3.\n"
                                     "Перечень файлов с ошибками:\n"};
        int successCount = totalNumOfFiles - problemCounter;
        m_importReportMessage.prepend(partialSuccessString.arg(totalNumOfFiles).
                                      arg(successCount).
                                      arg(problemCounter));
    }
}

void BrokerXmlClass::processErrorMessage(const QString &refFilename, const QString &refErrorDetails)
{
    QString fileReport{"\nФайл: %1 \nПроблема: %2\n"};
    m_importReportMessage.append(fileReport.arg(refFilename, refErrorDetails));
}

void BrokerXmlClass::slotSelectAndProcessDir(const QString &refDirpath)
{
    m_selectedDir = refDirpath;
    prepareFileList();
    processFiles();
}

void BrokerXmlClass::slotExportItemToXml(const QString &refSerializedData, const QString &refFilename) //add full address of created file.
{
    qDebug()<<__PRETTY_FUNCTION__<<"try to export"<<refSerializedData<<"to file:"<<refFilename;

    if(refFilename.isEmpty())
    {
        return;
    }
    QFile xmlExportFile(refFilename);
    if (xmlExportFile.open(QIODevice::WriteOnly))
    {
        QTextStream(&xmlExportFile)<<generateXmlFile(refSerializedData).toString();
        xmlExportFile.close();
    }
}

QDomDocument BrokerXmlClass::generateXmlFile(const QString &refSerializedData) const
{
    QDomDocument exportXmlDoc;
    QString data("version=\"1.0\" encoding=ASCII");
    QDomProcessingInstruction instr = exportXmlDoc.createProcessingInstruction("xml", data);
    exportXmlDoc.appendChild(instr);
    QDomElement domCoreElement = exportXmlDoc.createElement("root");
    exportXmlDoc.appendChild(domCoreElement);
    QStringList dataList = refSerializedData.split(":");
    size_t counter{0};

    for(auto &item : dataList)
    {
        QDomElement nodeBuffer;
        switch(counter)
        {
        case 0:
            nodeBuffer = exportXmlDoc.createElement("texteditor");
            break;
        case 1:
            nodeBuffer = exportXmlDoc.createElement("fileformats");
            break;
        case 2:
            nodeBuffer = exportXmlDoc.createElement("encoding");
            break;
        case 3:
            nodeBuffer = exportXmlDoc.createElement("hasintellisense");
            break;
        case 4:
            nodeBuffer = exportXmlDoc.createElement("hasplugins");
            break;
        case 5:
            nodeBuffer = exportXmlDoc.createElement("cancompile");
            break;
        }
        domCoreElement.appendChild(nodeBuffer);
        QDomText textElement = exportXmlDoc.createTextNode(item);
        nodeBuffer.appendChild(textElement);
        ++counter;
    }

    return exportXmlDoc;
}
