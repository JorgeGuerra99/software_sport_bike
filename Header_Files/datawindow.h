#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include "Header_Files/qcustomplot.h"
//Include header qcustom.h is not udes (fix available)
#include <iostream>
#include <vector>
#include "Header_Files/session.h"
#include "dialogwin.h"

using namespace std;
namespace Ui {
class datawindow;
}

class datawindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit datawindow(Session*&, QWidget *parent = nullptr);
    ~datawindow();
private:
    Ui::datawindow *ui;
    Session* ses;
    DialogWin* diaWin;
private slots:
    void ExportDataButton ();
};

#endif // DATAWINDOW_H
