

#include "Header_Files/weightwindow.h"
#include "ui_weightwindow.h"


weightwindow::weightwindow(User*& usu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::weightwindow)
{
    ui->setupUi(this);
    us = usu;
    wei = new WeightLoss (us->nameUsr, us->age, us->sex, us->weight, us->height);
    timer.setInterval(1000);
    connect(&timer, SIGNAL (timeout()), this, SLOT (UiSample()));
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (StartButton()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this,SLOT(OpenSessionButton()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT(PauseButton()));
    connect(ui->pushButton_4, SIGNAL (clicked()), this, SLOT(ReportButton()));
    connect(ui->pushButton_5, SIGNAL (clicked()), this, SLOT(StopButton()));
    connect(ui->pushButton_6, SIGNAL (clicked()), this, SLOT(deleteLater()));
    connect(ui->pushButton_7, SIGNAL (clicked()), this, SLOT(SerialConfigButton()));
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_5->setDisabled(true);
}

weightwindow::~weightwindow()
{
    delete ui;
    delete wei;
    cout << "destructor de weightwindow" << endl;
}

void weightwindow::StartButton()
{
    wei->Start();
    timer.start();
    ui->pushButton->setDisabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_7->setDisabled(true);

}

void weightwindow::PauseButton()
{
    if (pause)
    {
        pause = false;
        ui->pushButton_3->setText("Pausar entrenamiento");
        timer.start();
    }else
    {
        ui->pushButton_3->setText("Reanudar entrenamiento");
        timer.stop();
        pause = true;
    }
}

void weightwindow::StopButton()
{
    timer.stop();
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    wei->End();
    us->SaveLastSession (wei);
}
//
void weightwindow::UiSample()
{
    wei->Sample();
    ui->lcdNumber->display(wei->timeSes);
    ui->lcdNumber_2->display(wei->GetLastData(1));
    ui->lcdNumber_3->display(wei->GetDistance());
    ui->lcdNumber_4->display(wei->GetLastData(0));
    ui->lcdNumber_5->display(wei->GetLastData(2));
    ui->lcdNumber_6->display(wei->GetCalories());
    ui->label_7->setText(QString::fromStdString(wei->screenMessage));

}

void weightwindow::OpenSessionButton()
{
    wei->ReadReport();
}

void weightwindow::SerialConfigButton()
{
    Session* ptrSes;
    ptrSes = wei;
    confWin = new ConfigWindow (ptrSes, this);
    confWin->show();
}

void weightwindow::ReportButton()
{
    Session* ptrSes;
    ptrSes = wei;
    datWin = new datawindow (ptrSes, this);
    datWin->show();
}
