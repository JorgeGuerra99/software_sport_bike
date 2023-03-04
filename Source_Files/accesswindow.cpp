#include "Header_Files/accesswindow.h"
#include "ui_accesswindow.h"

accesswindow::accesswindow(User *us, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::accesswindow)
{
    ui->setupUi( this );
    usAccess = us;
    connect( ui->pushButton, SIGNAL( clicked() ), this, SLOT( AccessUser() ) );
    connect( ui->pushButton_2, SIGNAL( clicked() ), this, SLOT( CancelAccess() ) );
}

accesswindow::~accesswindow()
{
    delete ui;
}

void accesswindow::AccessUser ()
{
    string nameUser = ui->lineEdit->text().toStdString();
    // lineas que encripta la contraseña ingresada en md5
    QByteArray password = QByteArray( ui->lineEdit_2->text().toUtf8() );
    QString aux = QString( QCryptographicHash::hash( ( password ),QCryptographicHash::Md5 ).toHex() );
    string pass = aux.toStdString();
    try {
        usAccess = new User( nameUser, pass );
        emit UserAccess();
        this->close();
        delete this;
    }  catch ( int e ) {
        if ( e == US_PASS_INCORRECT )
        {
            DialogWin* msj;
            msj = new DialogWin( "Los datos ingresados no son válidos. Intente nuevamente" );
            msj->show();
        }
    }
}
