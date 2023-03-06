/**
 * @file accesswindow.h
 * @brief Clase de ventana de acceso de usuario (Ingreso de nombre de usuario y contraseña) (INTERFAZ GRÁFICA)
 * @date 2023
 * @authors Bazán María, Guerra Jorge
*/

#ifndef ACCESSWINDOW_H
#define ACCESSWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include <QByteArray>
#include <QCryptographicHash>
#include "user.h"
#include "dialogwin.h"

namespace Ui {
class accesswindow;
}

/**
 * @brief The accesswindow class: Clase de ventana de acceso a usuario (Interfaz gráfica)
 */

class accesswindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief accesswindow - Constructor
     * @param us: Objeto usuario donde se iniciará sesión
     * @param parent
     */
    explicit accesswindow (User* us, QWidget *parent = nullptr);
    ~accesswindow();
    /**
     * @brief usAccess: Objeto del tipo usuario para poder iniciar sesión
     */
    User *usAccess;

private:
    Ui::accesswindow *ui;

private slots:
    /**
     * @brief AccessUser : Slot a ejecutar por el botón de acceder
     * @note El slot toma los datos ingresados y crea el objeto User después de verificar password encriptada
     */
    void AccessUser ();
    /**
     * @brief CancelAccess: Elimina la ventana
     */
    void CancelAccess () { delete this; };
signals:
    /**
     * @brief UserAccess: Señal emitida cuando se accede exitosamente
     */
    void UserAccess ();
};

#endif // ACCESSWINDOW_H
