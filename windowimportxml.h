#ifndef WINDOWIMPORTXML_H
#define WINDOWIMPORTXML_H

#include <QDialog>

namespace Ui {
class windowImportXml;
}

class windowImportXml : public QDialog
{
    Q_OBJECT

public:
    explicit windowImportXml(QWidget *parent = nullptr);
    ~windowImportXml();

protected:
    void SetupView();
    void SetupConnections();
    virtual void closeEvent(QCloseEvent *pEvent = nullptr) override;

public slots:
    void slotSetProgressUpperLimit(size_t pbarUpperLimit);
    void slotSetCurrentProgress(size_t pbarCurrentProgress);
    void slotSetCurrentFilename(const QString &refImportedFilename);
    void slotReceiveReport(const QString &refImportReport);

signals:
    void signalClosed();
private:
    Ui::windowImportXml *ui;
};

#endif // WINDOWIMPORTXML_H
