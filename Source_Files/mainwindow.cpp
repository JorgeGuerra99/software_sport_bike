#include "Header_Files/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect (ui->pushButton, SIGNAL (clicked()), this, SLOT (buttReg()));
    connect (ui->pushButton_2, SIGNAL (clicked()), this, SLOT (buttAcc()));
}

MainWindow::~MainWindow()
{
    if (us != NULL) delete us;
    delete ui;
    cout << "Destructor de mainwindow" << endl;
    exit(1);
}

void MainWindow::buttReg()
{
   winReg = new RegWindow (us, this);
   connect(winReg, SIGNAL (UserRegistered()), this, SLOT (usReg()));
   winReg->show();
}

void MainWindow::usReg()
{
    us = winReg->usReg;
    QString nam;
    nam = QString::fromStdString(us->nameUsr);
    ui->label->setText(nam);
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
}

void MainWindow::usAcc()
{
    us = winAcc->usAccess;
    ui->label->setText(QString::fromStdString(us->nameUsr));
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
}

void MainWindow::buttAcc()
{
    winAcc = new accesswindow (us, this);
    connect (winAcc, SIGNAL (UserAccess()), this, SLOT (usAcc()));
    winAcc->show();
}



