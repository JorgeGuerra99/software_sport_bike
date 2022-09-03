//-------------------------------VENTANA DE REGISTRO DE USUARIO ----------------------------------------------
#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QMainWindow>
#include "user.h"

namespace Ui {
class RegWindow;
}

class RegWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegWindow(User*, QWidget *parent = nullptr);
    ~RegWindow();
    User *usReg;
private:
    Ui::RegWindow *ui;
private slots:
    void RegisterUser ();
    void CancelRegister () { delete this;}
signals:
    void UserRegistered ();
};

#endif // REGWINDOW_H
