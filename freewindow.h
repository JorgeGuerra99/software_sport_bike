/**
 * @file freewindow.h
 * @brief Ventana de sesión de entrenamiento del tipo Free (INTERFAZ GRÁFICA)
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef FREEWINDOW_H
#define FREEWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include "Header_Files/configwindow.h"
#include "Header_Files/datawindow.h"

namespace Ui {
class FreeWindow;
}

/**
 * @brief The FreeWindow class: Ventana de sesión de entrenamiento del tipo Free (Interfaz gráfica)
 */
class FreeWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief FreeWindow - Constructor
     * @param : Puntero a objeto Usuario
     * @param parent
     */
    FreeWindow(User*& usu , QWidget *parent = nullptr);
    ~FreeWindow();

private:
    Ui::FreeWindow *ui;
    /**
     * @brief fre: Puntero al objeto correspondiente a la sesión del tipo free
     */
    Free* fre;
    /**
     * @brief us: Puntero a objeto de usuario
     */
    User* us = nullptr;
    /**
     * @brief timer: Objeto utilizado como timer para la sesión
     */
    QTimer timer;
    /**
     * @brief pause: Flag indicador de pausa de entrenamiento
     */
    bool pause;
    /**
     * @brief confWin: Puntero a objeto de interfaz gráfica de la ventana de configuración del puerto serie
     */
    ConfigWindow* confWin;
    /**
     * @brief datWin: Puntero a objeto de interfaz gráfica de la ventana de reporte de datos
     */
    datawindow* datWin;
private slots:
    /**
     * @brief StartButton: SLOT asociado al botón de inicio de entrenamiento
     * @details Ejecuta los métodos de start de la sesión y el timer
     */
    void StartButton();
    /**
     * @brief PauseButton: SLOT asociado al botón de pausar/reanudar entrenamiento
     * @details Modifica el flag "pause" del objeto de sesión - Detiene/Inicia el timer
     */
    void PauseButton();
    /**
     * @brief StopButton: SLOT asociado al botón de detener entrenamiento
     * @details Detiene el timer y la sesión. Ejecuta el método de guardado de sesión
     */
    void StopButton();
    /**
     * @brief UiSample: SLOT asociado al timer. Se ejecuta cada vez que este finaliza su cuenta
     * @details Se usa para ejecutar el método de muestreo de la sesión
     */
    void UiSample ();
    /**
     * @brief SerialConfigButton: SLOT asociado al botón de configurar puerto serie
     * @details Crea y ejecuta la ventana de configuración del puerto serie
     */
    void SerialConfigButton ();
    /**
     * @brief ReportButton: SLOT asociado al botón de ver reporte
     * @details Crea y ejecuta la ventana de reporte de sesión
     */
    void ReportButton ();
signals:
    /**
     * @brief NewSession: Señal que es emitida para indicar que se realizó una sesión
     */
    void NewSession ();
};

#endif // FREEWINDOW_H
