#ifndef ONLYWINDOW_H
#define ONLYWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class OnlyWindow; }
QT_END_NAMESPACE

class OnlyWindow : public QMainWindow
{
    Q_OBJECT

public:
    OnlyWindow(QWidget *parent = nullptr);
    ~OnlyWindow();

private:
    Ui::OnlyWindow *ui;
};
#endif // ONLYWINDOW_H
