#include "Header_Files/mainsession.h"
#include "ui_mainsession.h"
#include <QTimer>

void MainSession::ActTimer()
{
    if (contSeg < 60)
    {
        ui->lcdNumber_2->display(contSeg++);
    }
    else
    {
        contSeg = 0;
        if(contMin < 60)
        {
            ui->lcdNumber_3->display(contMin++);
        }
        else
        {
            contMin = 0;
            if ( contHours < 24 )
            {
                ui->lcdNumber_4->display(contHours++);
            }
            else
            {
                contHours = 0;
            }
        }
    }
}

MainSession::MainSession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainSession)
{
    ui->setupUi(this);

}

MainSession::~MainSession()
{
    delete ui;
}

void MainSession::on_pushButton_2_clicked()
{

    QTimer *chronometer = new QTimer( this );
    connect( chronometer, SIGNAL(timeout()), this, SLOT(ActTimer()));
    chronometer->start(1000);
    flagTimer = true;
    if (flagTimer)
    {
        ui->pushButton_2->text();
    }

}

