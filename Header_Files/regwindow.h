/**
 * @file regwindow.h
 * @brief Ventana de registro de usuario (INTERFAZ GRÁFICA)
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QMainWindow>
#include <QCryptographicHash>
#include <QByteArray>
#include "user.h"

namespace Ui {
class RegWindow;
}

/**
 * @brief The RegWindow class: Ventana de registro de usuario (Interfaz gráfica)
 */
class RegWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief RegWindow - Constructor
     * @param us : Puntero a objeto User
     * @param parent
     */
    explicit RegWindow(User* us, QWidget *parent = nullptr);
    ~RegWindow();
    /**
     * @brief usReg: Puntero al objeto de usuario
     */
    User *usReg;
private:
    Ui::RegWindow *ui;
private slots:
    /**
     * @brief RegisterUser: SLOT asociado al botón de registrar usuario
     * @details Toma los datos ingresados por el usuario en la interfaz y los utiliza para crear el objeto usuario correspondiente
     */
    void RegisterUser ();
    /**
     * @brief CancelRegister: Cancelar registro y cierra la ventana
     */
    void CancelRegister () { delete this;}
signals:
    /**
     * @brief UserRegistered: Señal emitida cuando un usuario se registra exitosamente
     */
    void UserRegistered ();
};

#endif // REGWINDOW_H
