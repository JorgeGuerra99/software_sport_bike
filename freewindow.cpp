#include "freewindow.h"
#include "ui_freewindow.h"

FreeWindow::FreeWindow(User*& usu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FreeWindow)
{
    ui->setupUi(this);
    us = usu;
    fre = new Free (us->nameUsr, us->age, us->sex, us->weight, us->height);
    timer.setInterval(1000);
    connect(&timer, SIGNAL (timeout()), this, SLOT(UiSample()));
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT(StartButton()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT(PauseButton()));
    connect(ui->pushButton_5, SIGNAL (clicked()), this, SLOT(StopButton()));
    connect (ui->pushButton_6, SIGNAL (clicked()), this, SLOT (deleteLater()));
    connect(ui->pushButton_7, SIGNAL (clicked()), this, SLOT (SerialConfigButton()));
    connect(ui->pushButton_4, SIGNAL (clicked()), this, SLOT (ReportButton()));

    ui->pushButton_3->setDisabled(true);
    ui->pushButton_5->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
}


FreeWindow::~FreeWindow()
{
    delete fre;
    delete ui;
}

void FreeWindow::StartButton()
{
    fre->Start();
    timer.start();
    ui->pushButton->setDisabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

void FreeWindow::PauseButton()
{
    if (pause)
    {
        pause = false;
        ui->pushButton_3->setText("Pausar entrenamiento");
        timer.start();
    } else
    {
        ui->pushButton_3->setText("Reanudar entrenamiento");
        timer.stop();
        pause = true;
    }
}

void FreeWindow::StopButton()
{
    timer.stop();
    ui->pushButton_5->setDisabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_3->setDisabled(true);
    fre->End();
    us->SaveLastSession(fre);
}

void FreeWindow::UiSample()
{
    fre->Sample();
    ui->lcdNumber->display(fre->timeSes);
    ui->lcdNumber_2->display(fre->GetLastData(1));
    ui->lcdNumber_3->display(fre->GetDistance());
    ui->lcdNumber_4->display(fre->GetLastData(0));
    ui->lcdNumber_5->display(fre->GetLastData(2));
    ui->lcdNumber_6->display(fre->GetCalories());
    ui->label_7->setText(QString::fromStdString(fre->screenMessage));
}

void FreeWindow::ExportDataButton()
{
    fre->WriteReport();
}

void FreeWindow::SerialConfigButton()
{
    Session* ptrSes;
    ptrSes = fre;
    confWin = new ConfigWindow (ptrSes, this);
    confWin->show();
}

void FreeWindow::ReportButton()
{
    Session* ptrSes;
    ptrSes = fre;
    datWin = new datawindow (ptrSes, this);
    datWin->show();
}
