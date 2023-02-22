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
     * @brief ListPorts
     * @note Utilizado para llenar la lista de los puertos serie disponibles
     */
    void ListPorts ();
    Ui::ConfigWindow *ui;
    /**
     * @brief ses
     * @note puntero del objeto sesión
     */
    Session* ses;
    /**
     * @brief portList
     * @ note objeto utilizado para almacenar los puertos serie disponible
     */
    QStringList portList;
    int baud = QSerialPort::Baud9600;
private slots:
    /**
     * @brief BaudRateChanged
     * @param value : valor extraido del slider
     * @note SLOT asociado al slider de selección de baudRate
     */
    void BaudRateChanged (int value);
    /**
     * @brief SaveConfig
     * @note SLOT asociado al botón de guardar la configuración
     * @details asigna al puerto serie los valores seleccionado por el usuario
     */
    void SaveConfig ();
    /**
     * @brief Cancel
     * @note cierra la ventana de configuración del puerto serie
     */
    void Cancel () { this->close(); }
};

#endif // CONFIGWINDOW_H
