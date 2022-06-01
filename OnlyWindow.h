#ifndef ONLYWINDOW_H
#define ONLYWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>
#include "windowimportxml.h"

#include "model_editortable.h"

#include "delegateeditablerow.h"

#include "broker_sqlite.h"
#include "broker_xml.h"

#define COL_HAS_INTELLISENSE 3
#define COL_HAS_PLUGINS 4
#define COL_CAN_COMPILE 5

QT_BEGIN_NAMESPACE
namespace Ui { class OnlyWindow; }
QT_END_NAMESPACE

class OnlyWindow : public QMainWindow
{
    Q_OBJECT
private:
    BrokerXmlClass *m_ptrXmlBroker{nullptr};
    BrokerSqliteClass *m_ptrSqliteBroker{nullptr};
    Model_EditorTable *m_ptrModelData{nullptr};

    QFileDialog fileDirSelectDlg;
    windowImportXml xmlImportDlg;

    QMenu *m_objMenu{nullptr};
    QModelIndex selectedIndex;

    DelegateEditableRow *delegateEditable{nullptr};
    int m_editableRow{-1};
public:
    OnlyWindow(QWidget *parent = nullptr);
    OnlyWindow(Model_EditorTable *ptrModel, BrokerXmlClass* ptrXmlBroker, BrokerSqliteClass *ptrSqliteBroker, QWidget *parent = nullptr);
    ~OnlyWindow();

protected:
    void SetupConnections();
    void SetupContextMenu();

protected slots:
    void slotCustomMenuRequested(QPoint);
    void slotDeleteItem();
    void slotExportItem();
    void slotEditItem();

signals:
    void signalDeleteItem(int rowNumber);
    void signalExportItem(int rowNumber, const QString &refFilename);
    void signalEditItem(int rowNumber);
private:
    Ui::OnlyWindow *ui;
};
#endif // ONLYWINDOW_H

