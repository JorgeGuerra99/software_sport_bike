//----------------------------------VENTANA DE CONFIGURACIÓN DE PUERTO SERIE -----------------------------------------------
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "bike.h"
#include "session.h"
#include <vector>
namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QMainWindow
{
    Q_OBJECT


public:
    ConfigWindow(Session*&, QWidget *parent = nullptr);
    ~ConfigWindow();

private:
    Ui::ConfigWindow *ui;
    Session* ses;
    QStringList portList;
    void ListPorts ();
    int baud = QSerialPort::Baud9600;
private slots:
    void BaudRateChanged (int);
    void SaveConfig ();
    void Cancel () { this->close(); }
};

#endif // CONFIGWINDOW_H
