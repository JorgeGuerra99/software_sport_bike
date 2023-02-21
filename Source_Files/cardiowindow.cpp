#include "Header_Files/cardiowindow.h"
#include "ui_cardiowindow.h"

CardioWindow::CardioWindow(User*& usu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CardioWindow)
{
    us = usu;
    ui->setupUi(this);
    car = new Cardio (us->nameUsr, us->age, us->sex, us->weight, us->height);
    timer.setInterval(1000);
    connect(&timer, SIGNAL (timeout()), this, SLOT(UiSample()));
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT(StartButton()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this, SLOT(PauseButton()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT(StopButton()));
    connect (ui->pushButton_5, SIGNAL (clicked()), this, SLOT (deleteLater()));
    connect(ui->pushButton_6, SIGNAL (clicked()), this, SLOT (OpenSessionButton()));
    connect(ui->pushButton_7, SIGNAL (clicked()), this, SLOT (SerialConfigButton()));
    connect(ui->pushButton_8, SIGNAL (clicked()), this, SLOT (ReportButton()));

    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_8->setDisabled(true);

}

CardioWindow::~CardioWindow()
{
    delete ui;
    delete car;
    cout << "destructor de cardiowindow" << endl;
}

void CardioWindow::StartButton()
{
    car->Start();
    timer.start();
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

}

void CardioWindow::PauseButton()
{
    if (pause)
    {
        pause = false;
        ui->pushButton_2->setText("Pausar entrenamiento");
        timer.start();
    } else
    {
        ui->pushButton_2->setText("Reanudar entrenamiento");
        timer.stop();
        pause = true;
    }
}

void CardioWindow::StopButton()
{
    timer.stop();
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_8->setEnabled(true);
    car->End();
    us->SaveLastSession(car);
}

void CardioWindow::UiSample()
{
    car->Sample();
    ui->lcdNumber->display(car->timeSes);
    ui->lcdNumber_2->display(car->GetLastData(1));
    ui->lcdNumber_3->display(car->GetLastData(2));
    ui->lcdNumber_4->display(car->GetLastData(0));
    ui->lcdNumber_5->display(car->GetDistance());
    ui->lcdNumber_6->display(car->GetCalories());
    ui->label_5->setText(QString::fromStdString(car->screenMessage));
}

void CardioWindow::ExportDataButton()
{
    car->WriteReport();
}

void CardioWindow::OpenSessionButton()
{
    car->ReadReport();
}

void CardioWindow::SerialConfigButton()
{
    Session* ptrSes;
    ptrSes = car;
    confWin = new ConfigWindow (ptrSes, this);
    confWin->show();
}

void CardioWindow::ReportButton()
{
    Session* ptrSes;
    ptrSes = car;
    datWin = new datawindow (ptrSes, this);
    datWin->show();
}
