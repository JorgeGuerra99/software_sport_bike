//----------------------------------VENTANA DE CONFIGURACIÓN DE PUERTO SERIE -----------------------------------------------
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include "bike.h"
#include <vector>
namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private:
    Ui::ConfigWindow *ui;
    StateBike myBike;
    QStringList portList;
    void ListPorts ();
    int baud = QSerialPort::Baud9600;
private slots:
    void BaudRateChanged (int);
    void SaveConfig ();
    void Cancel () { this->close(); }
};

#endif // CONFIGWINDOW_H
