#ifndef ONLYWINDOW_H
#define ONLYWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>
#include "windowimportxml.h"

#include "model_editortable.h"

#include "broker_sqlite.h"
#include "broker_xml.h"

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

signals:
    void signalDeleteItem(int rowNumber);

private:
    Ui::OnlyWindow *ui;
};
#endif // ONLYWINDOW_H

