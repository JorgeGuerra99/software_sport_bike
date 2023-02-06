//-----------------------VENTANA DE ACCESO DE USUARIO -----------------------------------------------------------------
#ifndef ACCESSWINDOW_H
#define ACCESSWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QByteArray>
#include <QCryptographicHash>
#include "user.h"

namespace Ui {
class accesswindow;
}

class accesswindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit accesswindow (User*, QWidget *parent = nullptr);
    ~accesswindow();
    User *usAccess;

private:
    Ui::accesswindow *ui;

private slots:
    void AccessUser ();
    void CancelAccess () { delete this; };
signals:
    void UserAccess ();
};

#endif // ACCESSWINDOW_H
//hola
