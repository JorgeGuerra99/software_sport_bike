#ifndef CARDIOWINDOW_H
#define CARDIOWINDOW_H

#include <QMainWindow>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include <QTimer>

namespace Ui {
class CardioWindow;
}

class CardioWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CardioWindow(User* , QWidget *parent = nullptr);
    ~CardioWindow();

private:
    Ui::CardioWindow *ui;
    Cardio* car;
    QTimer timer;
private slots:
    void StartButton();
    void PauseButton(){};
    void StopButton(){};
    void UiSample ();
};

#endif // CARDIOWINDOW_H
