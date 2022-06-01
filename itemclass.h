    #ifndef ITEMCLASS_H
#define ITEMCLASS_H

#include <QObject>

#ifdef QT_DEBUG
#include <QDebug>
#endif

class EditorItemClass : public QObject
{
    Q_OBJECT

public:
    QString m_textEditor{""};
    QString m_fileFormats{""};
    QString m_encoding{""};
    bool m_hasIntellisense{false};
    bool m_hasPlugins{false};
    bool m_canCompile{false};

public:
    explicit EditorItemClass(QObject *parent = nullptr);
    explicit EditorItemClass(const QString &refSerializedData,
                             QObject *parent = nullptr);
    EditorItemClass(const EditorItemClass &other, QObject *parent = nullptr):QObject{parent}
    {
        m_textEditor = other.m_textEditor;
        m_fileFormats = other.m_fileFormats;
        m_encoding = other.m_encoding;
        m_hasIntellisense = other.m_hasIntellisense;
        m_hasPlugins = other.m_hasPlugins;
        m_canCompile = other.m_canCompile;
    }

    //example: "Notepad++;txt,xml,cpp;utf,ansi;true;true;false"
    QString toString() const;
    void fromString(const QString &refSerializedData, const QString &refSeparator = ":");

    EditorItemClass& operator=(const EditorItemClass &other)
    {
        m_textEditor = other.m_textEditor;
        m_fileFormats = other.m_fileFormats;
        m_encoding = other.m_encoding;
        m_hasIntellisense = other.m_hasIntellisense;
        m_hasPlugins = other.m_hasPlugins;
        m_canCompile = other.m_canCompile;
        return *this;
    }
signals:

};

#endif // ITEMCLASS_H
