#include "OnlyWindow.h"
#include "ui_OnlyWindow.h"
#include <QMessageBox>

OnlyWindow::OnlyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OnlyWindow)
{
    ui->setupUi(this);
}

OnlyWindow::OnlyWindow(ModelTableEditors *ptrModel, BrokerXmlClass* ptrXmlBroker, BrokerSqliteClass *ptrSqliteBroker,
                       QWidget *parent):OnlyWindow(parent)
{
    m_ptrModelData = ptrModel;
    m_ptrXmlBroker = ptrXmlBroker;
    m_ptrSqliteBroker = ptrSqliteBroker;

    if(m_ptrModelData!=nullptr)
    {
        ui->tviewSqlTable->setModel(m_ptrModelData);
        delegateEditable = new DelegateEditableRow();
        ui->tviewSqlTable->setItemDelegate(delegateEditable);
        ui->tviewSqlTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

//    xmlImportDlg.setModal(true);
    fileDirSelectDlg.setFileMode(QFileDialog::Directory);
    SetupConnections();
    SetupContextMenu();
}

OnlyWindow::~OnlyWindow()
{
    delete ui;
}

void OnlyWindow::SetupConnections()
{
    //buttons
    connect(ui->tbuttonImport, SIGNAL(clicked(bool)),
            &fileDirSelectDlg, SLOT(show()));
    connect(ui->tbuttonClearTable, SIGNAL(clicked(bool)),
            m_ptrModelData, SLOT(slotClearModel()));
    connect(ui->tbuttonClearTable, SIGNAL(clicked(bool)),
            SLOT(slotEditItem()));//to drop edit selection

    //xml import report dialog
    connect(&fileDirSelectDlg, SIGNAL(fileSelected(QString)),
            m_ptrXmlBroker, SLOT(slotSelectAndProcessDir(QString)));
    connect(m_ptrXmlBroker, SIGNAL(signalProgressUpperLimit(size_t)),
            &xmlImportDlg, SLOT(slotSetProgressUpperLimit(size_t)));
    connect(m_ptrXmlBroker, SIGNAL(signalCurrentProgress(size_t)),
            &xmlImportDlg, SLOT(slotSetCurrentProgress(size_t)));
    connect(m_ptrXmlBroker, SIGNAL(signalCurrentFilename(QString)),
            &xmlImportDlg, SLOT(slotSetCurrentFilename(QString)));
    connect(m_ptrXmlBroker, SIGNAL(signalSendReport(QString)),
            &xmlImportDlg, SLOT(slotReceiveReport(QString)));

    //context menu
    ui->tviewSqlTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tviewSqlTable, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(slotCustomMenuRequested(QPoint)));

    //connect with model and delegade
    connect(this, SIGNAL(signalDeleteItem(int)),
            m_ptrModelData, SLOT(slotDeleteItem(int)));
    connect(this, SIGNAL(signalExportItem(int,QString)),
            m_ptrModelData, SLOT(slotExportItem(int,QString)));
    connect(m_ptrModelData, SIGNAL(signalExportItem(QString,QString)),
            m_ptrXmlBroker, SLOT(slotExportItemToXml(QString,QString)));
    connect(this, SIGNAL(signalEditItem(int)),
            m_ptrModelData, SLOT(slotEditItem(int)));
    connect(this, SIGNAL(signalEditItem(int)),
            delegateEditable, SLOT(slotSetEditableRow(int)));
}

void OnlyWindow::SetupContextMenu()
{
    m_objMenu=new QMenu(this);

    QAction *editItem = new QAction("Edit Item", this);
    connect(editItem, SIGNAL(triggered()),this, SLOT(slotEditItem()));
    QAction *exportItem = new QAction("Export Item", this);
    connect(exportItem, SIGNAL(triggered()),this, SLOT(slotExportItem()));
    QAction *deleteItem = new QAction("Delete Item", this);
    connect(deleteItem, SIGNAL(triggered()),this, SLOT(slotDeleteItem()));

    m_objMenu->addAction(editItem);
    m_objMenu->addAction(exportItem);
    m_objMenu->addSeparator();
    m_objMenu->addAction(deleteItem);
}

void OnlyWindow::slotCustomMenuRequested(QPoint pos)
{
    selectedIndex=ui->tviewSqlTable->indexAt(pos);

    if(selectedIndex.row()>-1)
    {
        if(selectedIndex.row() == m_editableRow)//same row clicked
        {
            m_objMenu->actions().at(0)->setText("Drop Edit Item Selection");
        }
        else
        {
            m_objMenu->actions().at(0)->setText("Edit Item");
        }
        m_objMenu->actions().at(0)->setEnabled(true);
        m_objMenu->actions().at(1)->setEnabled(true);
        m_objMenu->actions().at(3)->setEnabled(true);
    }
    else
    {
        if(m_editableRow>-1)//smth is edited
        {
            m_objMenu->actions().at(0)->setText("Drop Edit Item Selection");
            m_objMenu->actions().at(0)->setEnabled(true);
        }
        else
        {
            m_objMenu->actions().at(0)->setText("Edit Item");
            m_objMenu->actions().at(0)->setEnabled(false);
        }
        m_objMenu->actions().at(1)->setEnabled(false);
        m_objMenu->actions().at(3)->setEnabled(false);
    }

    if(m_objMenu!=nullptr)
        m_objMenu->popup(ui->tviewSqlTable->viewport()->mapToGlobal(pos));
}

void OnlyWindow::slotDeleteItem()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление", "Удалить запись?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        emit signalDeleteItem(selectedIndex.row());
        m_editableRow = -1;
        emit signalEditItem(m_editableRow);
    }

}

void OnlyWindow::slotExportItem()
{
    QString exportFilename;

    exportFilename = QFileDialog::getSaveFileName(this, "Экспорт файла","",
                                "xml-файл (*.xml)");

    if(!exportFilename.isEmpty())
    {
        emit signalExportItem(selectedIndex.row(), exportFilename);
    }
}

void OnlyWindow::slotEditItem()
{
    if(selectedIndex.row() == m_editableRow)
    {
        m_editableRow = -1;
        emit signalEditItem(m_editableRow);
        return;
    }

    emit signalEditItem(selectedIndex.row());

    if(selectedIndex.row()>-1)
    {
        m_editableRow = selectedIndex.row();
    }
    else
    {
        m_editableRow = -1;
    }
}

