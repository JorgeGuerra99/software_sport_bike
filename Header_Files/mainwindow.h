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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief us
     * @note puntero del objeto usuario
     */
    User *us = nullptr;

private:
    Ui::MainWindow *ui;
    /**
     * @brief winReg
     * @note puntero del objeto ventana de registro
     */
    RegWindow *winReg;
    /**
     * @brief winAcc
     * @note puntero del objeto ventana de acceso
     */
    accesswindow *winAcc;
    /**
     * @brief winCar
     * @note puntero del objeto ventana de sesión Cardio
     */
    CardioWindow *winCar;
    /**
     * @brief winWei
     * @note puntero del objeto ventana de sesión WeightLoss
     */
    weightwindow *winWei;
    /**
     * @brief winFre
     * @note puntero del objeto ventana de la sesión Free
     */
    FreeWindow *winFre;
    datawindow *winData;
    QStringList nameSessions;
private slots:
    /**
     * @brief buttReg
     * @note SLOT asociado al botón de registrar un nuevo usuario
     * @details crea y ejecuta la ventana regwindow
     */
    void ButtReg ();
    /**
     * @brief usReg
     * @note SLOT asociado a la señal de usuario registrado
     * @details asigna al punterio del objeto usuario la información ingresada en la ventana correspondiente
     */
    void UsReg ();
    /**
     * @brief buttAcc
     * @note SLOT asociado al botón de acceder a un usuario ya existente
     * @details crea y ejecita la ventana de accesswindow
     */
    void ButtAcc ();
    /**
     * @brief usAcc
     * @note SLOT asociado a la señal de ingreso de un usuario existente
     * @details asigna al puntero del objeto usuario la información obtenida de la ventana correspondiente
     */
    void UsAcc ();
    /**
     * @brief startSession
     * @note SLOT asociado al botón iniciar entrenamiento
     * @details crea y ejecuta la ventana correspondiente a la sesión elegida por el usuario
     */
    void StartSession();
    void ViewReport();
    void RefreshSession ();
    /**
     * @brief closedWindow
     * @note cierra el programa
     */
    void ClosedWindow () {cout << "ventana cerrada" << endl;}
};
#endif // MAINWINDOW_H
