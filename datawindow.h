#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include "Header_Files/qcustomplot.h"
#include <iostream>
#include <vector>
#include "Header_Files/session.h"

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
};

#endif // DATAWINDOW_H
