#include "dialogwin.h"
#include "ui_dialogwin.h"

DialogWin::DialogWin(string msj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWin)
{
    ui->setupUi(this);
    ui->label->setText(QString::fromStdString(msj));
}

DialogWin::~DialogWin()
{
    delete ui;
}
