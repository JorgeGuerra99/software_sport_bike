#include "Header_Files/mainsession.h"
#include "ui_mainsession.h"

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

