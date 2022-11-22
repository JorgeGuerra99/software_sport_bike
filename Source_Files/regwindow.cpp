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
    try {
        usReg = new User (nameUser, ag, sex, we, he, pass);
        emit UserRegistered();
        this->close();
        delete this;
    }  catch (int e) {
        if (e == REGISTERED_USER)
        {
            ui->label_8->setText("ERROR: Este nombre de usuario ya se encuentra registrado");
        }
        if (e == FILE_ERROR)
        {
            ui->label_8->setText("Error abriendo UserData.txt");
        }

    }
}
