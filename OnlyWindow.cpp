#include "OnlyWindow.h"
#include "ui_OnlyWindow.h"

OnlyWindow::OnlyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OnlyWindow)
{
    ui->setupUi(this);
}

OnlyWindow::~OnlyWindow()
{
    delete ui;
}

