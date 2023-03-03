/**
 * @file datawindow.h
 * @brief Ventana de muestra de datos de entrenamiento (INTERFAZ GRÁFICA)
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef DATAWINDOW_H
#define DATAWINDOW_H
#include <QMainWindow>
#include <iostream>
#include <vector>
#include "Header_Files/qcustomplot.h"
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
    /**
     * @brief datawindow - Constructor
     * @param mySes: Puntero a objeto de Session (clase base)
     * @param parent
     */
    explicit datawindow(Session*& mySes, QWidget *parent = nullptr);
    ~datawindow();
private:
    Ui::datawindow *ui;
    /**
     * @brief ses: Puntero a objeto de sesión
     */
    Session* ses;
    /**
     * @brief diaWin: Puntero a objeto de ventana de cuadro de diálogo
     */
    DialogWin* diaWin;
private slots:
    /**
     * @brief ExportDataButton: SLOT asociado al botón de Exportar datos
     * @details Ejecuta el método WriteReport de la sesión
     */
    void ExportDataButton ();
};

#endif // DATAWINDOW_H
