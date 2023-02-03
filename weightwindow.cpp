#include "weightwindow.h"
#include "ui_weightwindow.h"


weightwindow::weightwindow(User* us, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weightwindow)
{
    ui->setupUi(this);
    wei = new WeightLoss (us->nameUsr, us->age, us->sex, us->weight, us->height);
    timer.setInterval(1000);
    connect(&timer, SIGNAL (timeout()), this, SLOT (UiSample()));
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (StartButton()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this,SLOT(OpenSessionButton()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT(PauseButton()));
    connect(ui->pushButton_4, SIGNAL (clicked()), this, SLOT(ExportDataButton()));
    connect(ui->pushButton_5, SIGNAL (clicked()), this, SLOT(StopButton()));
    connect(ui->pushButton_6, SIGNAL (clicked()), this, SLOT(deleteLater()));
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_5->setDisabled(true);
}

weightwindow::~weightwindow()
{
    delete ui;
}
