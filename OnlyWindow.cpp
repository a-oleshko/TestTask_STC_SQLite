#include "OnlyWindow.h"
#include "ui_OnlyWindow.h"

OnlyWindow::OnlyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OnlyWindow)
{
    ui->setupUi(this);
}

OnlyWindow::OnlyWindow(Model_EditorTable *ptrModel, BrokerXmlClass* ptrXmlBroker, BrokerSqliteClass *ptrSqliteBroker,
                       QWidget *parent):OnlyWindow(parent)
{
    m_ptrModelData = ptrModel;
    m_ptrXmlBroker = ptrXmlBroker;
    m_ptrSqliteBroker = ptrSqliteBroker;

    if(m_ptrModelData!=nullptr)
    {
        ui->tviewSqlTable->setModel(m_ptrModelData);
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

    //    //xml import report dialog
    connect(&fileDirSelectDlg, SIGNAL(fileSelected(const QString&)),
            m_ptrXmlBroker, SLOT(slotSelectAndProcessDir(const QString&)));
    connect(m_ptrXmlBroker, SIGNAL(signalProgressUpperLimit(size_t)),
            &xmlImportDlg, SLOT(slotSetProgressUpperLimit(size_t)));
    connect(m_ptrXmlBroker, SIGNAL(signalCurrentProgress(size_t)),
            &xmlImportDlg, SLOT(slotSetCurrentProgress(size_t)));
    connect(m_ptrXmlBroker, SIGNAL(signalCurrentFilename(const QString &)),
            &xmlImportDlg, SLOT(slotSetCurrentFilename(const QString &)));
    connect(m_ptrXmlBroker, SIGNAL(signalSendReport(const QString &)),
            &xmlImportDlg, SLOT(slotReceiveReport(const QString &)));


    //context menu
    ui->tviewSqlTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tviewSqlTable, SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(slotCustomMenuRequested(QPoint)));

    //connect with model
    connect(this, SIGNAL(signalDeleteItem(int)),
            m_ptrModelData, SLOT(slotDeleteItem(int)));
}

void OnlyWindow::SetupContextMenu()
{
    m_objMenu=new QMenu(this);

    QAction *editItem = new QAction("Edit Item", this);
    QAction *exportItem = new QAction("Export Item", this);
    QAction *deleteItem = new QAction("Delete Item", this);
    connect(deleteItem, SIGNAL(triggered()),
            this, SLOT(slotDeleteItem()));

    m_objMenu->addAction(editItem);
    m_objMenu->addAction(exportItem);
    m_objMenu->addSeparator();
    m_objMenu->addAction(deleteItem);
}

void OnlyWindow::slotCustomMenuRequested(QPoint pos)
{
    selectedIndex=ui->tviewSqlTable->indexAt(pos);
    if(m_objMenu!=nullptr)
        m_objMenu->popup(ui->tviewSqlTable->viewport()->mapToGlobal(pos));
}

void OnlyWindow::slotDeleteItem()
{
    emit signalDeleteItem(selectedIndex.row());
}

