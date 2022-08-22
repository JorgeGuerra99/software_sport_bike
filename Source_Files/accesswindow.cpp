#include "Header_Files/accesswindow.h"
#include "ui_accesswindow.h"

accesswindow::accesswindow(User *us, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::accesswindow)
{
    ui->setupUi(this);
    usAccess = us;
    connect( ui->pushButton, SIGNAL (clicked()), this, SLOT (AccessUser()));
    connect( ui->pushButton_2, SIGNAL (clicked()), this, SLOT (CancelAccess()));
}

accesswindow::~accesswindow()
{
    delete ui;
    cout << "x aca tb" << endl;
}

void accesswindow::AccessUser ()
{
    string nameUser = ui->lineEdit->text().toStdString();
    string pass = ui->lineEdit_2->text().toStdString();
    try {
        usAccess = new User (nameUser, pass);
        emit UserAccess();
        this->close();
        delete this;
    }  catch (int e) {
        if ( e == US_PASS_INCORRECT)
        {
            ui->label_2->setText("Los datos ingresados no son válidos. Intente nuevamente.");
        }
    }
}
