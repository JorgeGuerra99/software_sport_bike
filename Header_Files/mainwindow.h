/**
* @file mainwindow.h
* @brief Ventana principal del programa
* @date 2023
* @authors Bazán María, Guerra Jorge
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "user.h"
#include "regwindow.h"
#include "accesswindow.h"
#include "cardiowindow.h"
#include "weightwindow.h"
#include "freewindow.h"
#include "datawindow.h"
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow - Constructor
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief us: Puntero del objeto usuario
     */
    User *us = nullptr;

private:
    Ui::MainWindow *ui;
    /**
     * @brief winReg: Puntero del objeto ventana de registro
     */
    RegWindow *winReg;
    /**
     * @brief winAcc: Puntero del objeto ventana de acceso
     */
    accesswindow *winAcc;
    /**
     * @brief winCar: Puntero del objeto ventana de sesión Cardio
     */
    CardioWindow *winCar;
    /**
     * @brief winWei: Puntero del objeto ventana de sesión WeightLoss
     */
    weightwindow *winWei;
    /**
     * @brief winFre: Puntero del objeto ventana de la sesión Free
     */
    FreeWindow *winFre;
    /**
     * @brief winData: Puntero del objeto ventana de mostrar datos
     */
    datawindow *winData;

private slots:
    /**
     * @brief buttReg: SLOT asociado al botón de registrar un nuevo usuario
     * @details Crea y ejecuta la ventana regwindow
     */
    void ButtReg ();
    /**
     * @brief usReg: SLOT asociado a la señal de usuario registrado
     * @details Asigna al punterio del objeto usuario la información ingresada en la ventana correspondiente
     */
    void UsReg ();
    /**
     * @brief buttAcc: SLOT asociado al botón de acceder a un usuario ya existente
     * @details Crea y ejecita la ventana de accesswindow
     */
    void ButtAcc ();
    /**
     * @brief usAcc: SLOT asociado a la señal de ingreso de un usuario existente
     * @details Asigna al puntero del objeto usuario la información obtenida de la ventana correspondiente
     */
    void UsAcc ();
    /**
     * @brief startSession: SLOT asociado al botón iniciar entrenamiento
     * @details Crea y ejecuta la ventana correspondiente a la sesión elegida por el usuario
     */
    void StartSession();
    /**
     * @brief ViewReport: SLOT asociado al botón de ver reporte
     * @details Crea y ejecuta la ventana correspondiente para mostrar los datos de sesión elegida por el usuario
     */
    void ViewReport();
    /**
     * @brief RefreshSession: SLOT asociado a actualizar la lista de sesiones realizadas por el usuario
     * @details Se utiliza al detectar la señal emitia por las ventanas correspondientes a las sesiones permitiendo agregar la que sesión que acaba de terminar de realizar el usuario
     */
    void RefreshSession ();
    /**
     * @brief closedWindow: Cierra el programa
     */
    void ClosedWindow () {cout << "ventana cerrada" << endl;}
};
#endif // MAINWINDOW_H
