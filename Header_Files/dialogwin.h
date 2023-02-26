/**
* @file dialogwin.h
* @brief Ventana para mostrar mensajes al usuario
* @date 2023
* @authors Bazán María, Guerra Jorge
*/
#ifndef DIALOGWIN_H
#define DIALOGWIN_H
#include <QDialog>
#include <string>

using namespace std;

namespace Ui {
class DialogWin;
}

class DialogWin : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DialogWin - Constructor
     * @param msj mensaje que se le desea mostrar al usuario
     * @param parent
     */
    DialogWin(string msj, QWidget *parent = nullptr);
    ~DialogWin();

private:
    Ui::DialogWin *ui;
};

#endif // DIALOGWIN_H
