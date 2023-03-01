#include "Header_Files/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect (ui->pushButton, SIGNAL (clicked()), this, SLOT (ButtReg()));
    connect (ui->pushButton_2, SIGNAL (clicked()), this, SLOT (ButtAcc()));
    connect (ui->pushButton_4, SIGNAL (clicked()), this, SLOT (StartSession()));
    connect (ui->pushButton_5, SIGNAL (clicked()), this, SLOT (ViewReport()));
}

MainWindow::~MainWindow()
{
    if (us != NULL) delete us;
    delete ui;
    cout << "Destructor de mainwindow" << endl;
    exit(1);
}

void MainWindow::ButtReg()
{
   winReg = new RegWindow (us, this);
   connect(winReg, SIGNAL (UserRegistered()), this, SLOT (UsReg()));
   winReg->show();
}

void MainWindow::UsReg()
{
    us = winReg->usReg;
    QString auxNameSession;
    vector<string> aux = us->NameSessions();
    for (int i =0 ; i< (int)aux.size();i++)
    {
      auxNameSession = QString::fromStdString(aux[i]);
      ui->comboBox_2->addItem(auxNameSession);
    }
    QString nam;
    nam = QString::fromStdString(us->nameUsr);
    ui->label->setText(nam);
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

void MainWindow::UsAcc()
{
    us = winAcc->usAccess;
    ui->label->setText(QString::fromStdString(us->nameUsr));
    QString auxNameSession;
    vector<string> aux = us->NameSessions();
    for (int i =0 ; i< (int)aux.size();i++)
    {
      auxNameSession = QString::fromStdString(aux[i]);
      ui->comboBox_2->addItem(auxNameSession);
    }
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

void MainWindow::StartSession()
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
        winWei = new weightwindow (us, this);
        winWei->show();
        break;
    case 2:
        cout << "Seleccionado libre" << endl;
        winFre = new FreeWindow (us, this);
        winFre->show ();
        break;
    default:
        cout << "No se ha seleccionado nada" << endl;
        break;
    }
}

void MainWindow::ButtAcc()
{
    winAcc = new accesswindow (us, this);
    connect (winAcc, SIGNAL (UserAccess()), this, SLOT (UsAcc()));
    winAcc->show();
}

void MainWindow::ViewReport()
{
    const int indice = ui->comboBox_2->currentIndex();
    Session* auxSes = us->GetSession(indice);
    winData = new datawindow (auxSes);
    winData->show();
}


