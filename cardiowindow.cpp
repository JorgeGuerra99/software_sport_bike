#include "cardiowindow.h"
#include "ui_cardiowindow.h"

CardioWindow::CardioWindow(User* us, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CardioWindow)
{
    ui->setupUi(this);
    car = new Cardio (us->age, us->sex);
    timer.setInterval(1000);
    connect(&timer, SIGNAL (timeout()), this, SLOT(UiSample()));
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT(StartButton()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this, SLOT(PauseButton()));
    connect(ui->pushButton_3, SIGNAL (clicked()), this, SLOT(StopButton()));

}

CardioWindow::~CardioWindow()
{
    delete ui;
    delete car;
}

void CardioWindow::StartButton()
{
    car->Start();
    timer.start();
}

void CardioWindow::UiSample()
{
    car->Sample();
    ui->lcdNumber->display(car->time);
}
