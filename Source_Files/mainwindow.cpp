#include "Header_Files/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect (ui->pushButton, SIGNAL (clicked()), this, SLOT (buttReg()));
    connect (ui->pushButton_2, SIGNAL (clicked()), this, SLOT (buttAcc()));
    connect (ui->pushButton_4, SIGNAL (clicked()), this, SLOT (startSession()));
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
    ui->pushButton_4->setEnabled(true);
    ui->comboBox->setEnabled(true);
}

void MainWindow::usAcc()
{
    us = winAcc->usAccess;
    ui->label->setText(QString::fromStdString(us->nameUsr));
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->comboBox->setEnabled(true);
}

void MainWindow::startSession()
{
    const int selectedSession = ui->comboBox->currentIndex();
    switch (selectedSession) {
    case 0:
        cout << "Seleccionado cardio" << endl;
        winCar = new CardioWindow (us, this);
        winCar->show();
        break;
    case 1:
        cout << "Seleccionado perdida de peso" << endl;
        break;
    case 2:
        cout << "Seleccionado libre" << endl;
        break;
    default:
        cout << "No se ha seleccionado nada" << endl;
        break;
    }
}

void MainWindow::buttAcc()
{
    winAcc = new accesswindow (us, this);
    connect (winAcc, SIGNAL (UserAccess()), this, SLOT (usAcc()));
    winAcc->show();
}



