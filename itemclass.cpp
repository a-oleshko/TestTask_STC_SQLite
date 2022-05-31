#include "itemclass.h"

EditorItemClass::EditorItemClass(const QString &refSerializedData,
                     QObject *parent)
    : QObject{parent}
{
    try
    {
        fromString(refSerializedData);
    }
    catch(int errNo)
    {
        throw;
    }
}

QString EditorItemClass::toString() const
{
    QString retString;
    retString.append(m_textEditor).append(";").append(m_fileFormats).append(";")
            .append(m_encoding).append(";").append(m_hasIntellisense?"true":"false").append(";")
            .append(m_hasPlugins?"true":"false").append(";").append(m_canCompile?"true":"false");
    return retString;

}

void EditorItemClass::fromString(const QString &refSerializedData,
                           const QString &refSeparator)
{
    QStringList list = refSerializedData.split(refSeparator);
    if(list.size()<6)
    {
        throw -1;//insufficient data
    }
    else
    {
        m_textEditor = list.at(0);
        m_fileFormats = list.at(1);
        m_encoding = list.at(2);
        m_hasIntellisense = list.at(3).toLower().contains("true");
        m_hasPlugins = list.at(4).toLower().contains("true");
        m_canCompile = list.at(5).toLower().contains("true");

        if(m_textEditor.isEmpty()||m_fileFormats.isEmpty()||m_encoding.isEmpty())
        {
            throw -2; //empty data
        }
    }
}
