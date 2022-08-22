#include "Header_Files/regwindow.h"
#include "ui_regwindow.h"

RegWindow::RegWindow(User *us, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegWindow)
{
    ui->setupUi(this);
    usReg = us;
    connect(ui->pushButton, SIGNAL (clicked()), this, SLOT (RegisterUser()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this, SLOT (CancelRegister()));
}

RegWindow::~RegWindow()
{
    delete ui;
    cout << "angora fudeu" << endl;
}

void RegWindow::RegisterUser()
{
    string nameUser = ui->lineEdit->text().toStdString();
    string pass = ui->lineEdit_2->text().toStdString();
    int ag = ui->spinBox->value();
    float we = ui->spinBox_2->value();
    float he = ui->spinBox_3->value();
    char sex;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        sex = 'M';
        break;
    case 1:
        sex = 'F';
        break;
    }
    usReg = new User (nameUser, ag, sex, we, he, pass);
    emit UserRegistered();
    this->close();
}
