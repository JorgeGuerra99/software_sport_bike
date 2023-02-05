#include "weightwindow.h"
#include "ui_weightwindow.h"


weightwindow::weightwindow(User* us, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::weightwindow)
{
    ui->setupUi(this);
    wei = new WeightLoss (*us);
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
    wei->End();
}
//
void weightwindow::UiSample()
{
    wei->Sample();
    ui->lcdNumber->display(wei->timeSes);
    ui->lcdNumber_2->display(wei->velocData.back());
    ui->lcdNumber_3->display(wei->distance);
    ui->lcdNumber_4->display(wei->pulseData.back());
    ui->lcdNumber_5->display(wei->dataOfLoad.back());
    ui->lcdNumber_6->display(wei->calories);
    ui->label_7->setText(QString::fromStdString(wei->screenMessage));

}

void weightwindow::ExportDataButton()
{
    wei->WriteReport();
}

void weightwindow::OpenSessionButton()
{
    wei->ReadReport();
}

