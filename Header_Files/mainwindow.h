//---------------------------------- VENTANA INICIAL -------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "regwindow.h"
#include "accesswindow.h"
#include "cardiowindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    User *us = nullptr; //objeto User vacío
    RegWindow *winReg;
    accesswindow *winAcc;
    CardioWindow *winCar;

private:
    Ui::MainWindow *ui;
private slots:
    void buttReg ();
    void usReg ();
    void buttAcc ();
    void usAcc ();
    void startSession();
    void closedWindow () {cout << "ventana cerrada" << endl;}
};
#endif // MAINWINDOW_H
