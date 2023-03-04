/**
 * @file cardiowindow.h
 * @brief Ventana de sesión de entrenamiento del tipo Cardio (INTERFAZ GRÁFICA)
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef CARDIOWINDOW_H
#define CARDIOWINDOW_H

#include <QMainWindow>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include "Header_Files/configwindow.h"
#include "datawindow.h"
#include <QTimer>

namespace Ui {
class CardioWindow;
}

/**
 * @brief The CardioWindow class
 */
class CardioWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
      * @brief CardioWindow - Constructor
      * @param usu : Puntero a objeto Usuario
      * @param parent
      */
     CardioWindow(User*& usu, QWidget *parent = nullptr);
    ~CardioWindow();

private:
    Ui::CardioWindow *ui;
    /**
     * @brief us: Puntero a objeto de usuario
     */
    User* us = nullptr;
    /**
     * @brief car: Puntero a objeto de sesión de entrenamiento (Cardio)
     */
    Cardio* car;
    /**
     * @brief timer: Objeto utilizado como timer para la sesión
     */
    QTimer timer;
    /**
     * @brief pause: Flag indicador de pausa de entrenamiento
     */
    bool pause = false;
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
     * @brief NewSession: Señal emitida cuando se ha finalizado una nueva sesión
     */
    void NewSession ();
};

#endif // CARDIOWINDOW_H
