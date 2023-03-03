/**
 * @file configwindow.h
 * @brief Ventana de configuración del puerto serie
 * @date 2023
 * @authors Bazán María, Guerra Jorge
*/

#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include "session.h"
#include <vector>
namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QMainWindow
{
    Q_OBJECT


public:
    /**
     * @brief ConfigWindow - Constructor
     * @param sesBike : Puntero a objeto Sesión
     * @param parent
     */
    ConfigWindow(Session*& sesBike, QWidget *parent = nullptr);
    ~ConfigWindow();
private:
    /**
     * @brief ListPorts: Utilizado para llenar la lista de los puertos serie disponibles
     */
    void ListPorts ();
    Ui::ConfigWindow *ui;
    /**
     * @brief ses: Puntero del objeto sesión
     */
    Session* ses;
    /**
     * @brief portList: Objeto utilizado para almacenar los puertos serie disponible
     */
    QStringList portList;
    /**
     * @brief baud: Es el valor de baudios por defectos que se muestra y utiliza el puerto serie para configurarlo
     */
    int baud = QSerialPort::Baud9600;
private slots:
    /**
     * @brief BaudRateChanged: SLOT asociado al slider de selección de baudRate
     * @param value: Valor extraido del slider
     */
    void BaudRateChanged (int value);
    /**
     * @brief SaveConfig: SLOT asociado al botón de guardar la configuración
     * @details asigna al puerto serie los valores seleccionado por el usuario
     */
    void SaveConfig ();
    /**
     * @brief Cancel: Cierra la ventana de configuración del puerto serie
     */
    void Cancel () { this->close(); }
};

#endif // CONFIGWINDOW_H
