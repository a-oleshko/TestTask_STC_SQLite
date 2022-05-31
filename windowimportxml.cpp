#include "windowimportxml.h"
#include "ui_windowimportxml.h"

windowImportXml::windowImportXml(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowImportXml)
{
    ui->setupUi(this);
    SetupConnections();
}

windowImportXml::~windowImportXml()
{
    delete ui;
}

void windowImportXml::SetupView()
{

}

void windowImportXml::SetupConnections()
{
    connect(ui->tbuttonOK, SIGNAL(clicked(bool)),
            this, SLOT(close()));
}

void windowImportXml::closeEvent(QCloseEvent *pEvent)
{
    emit signalClosed();
}

void windowImportXml::slotSetProgressUpperLimit(size_t pbarUpperLimit)
{
    ui->pbarImportProgress->setMaximum(pbarUpperLimit>1?pbarUpperLimit:1);
    show();
}

void windowImportXml::slotSetCurrentProgress(size_t pbarCurrentProgress)
{
    ui->pbarImportProgress->setValue(pbarCurrentProgress);
}

void windowImportXml::slotSetCurrentFilename(const QString &refImportedFilename)
{
    ui->lblCurrentFilename->clear();
    ui->lblCurrentFilename->setText(refImportedFilename);
}

void windowImportXml::slotReceiveReport(const QString &refImportReport)
{
    ui->lblReport->clear();
    ui->lblCurrentFilename->clear();
    ui->lblReport->setText(refImportReport);
}
