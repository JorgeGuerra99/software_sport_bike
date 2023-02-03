#ifndef WEIGHTWINDOW_H
#define WEIGHTWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include <QTimer>

namespace Ui {
class weightwindow;
}

class weightwindow : public QWidget
{
    Q_OBJECT

public:
    weightwindow(User* , QWidget *parent = nullptr);
    ~weightwindow();

private:
    Ui::weightwindow *ui;
    WeightLoss* wei;
    QTimer timer;
    bool pause;

private slots:
    void StartButton ();
    void PauseButton ();
    void StopButton ();
    void UiSample ();
    void ExportDataButton ();
    void OpenSessionButton ();

};

#endif // WEIGHTWINDOW_H
