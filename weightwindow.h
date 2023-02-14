#ifndef WEIGHTWINDOW_H
#define WEIGHTWINDOW_H

#include <QMainWindow>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include "Header_Files/configwindow.h"
#include "datawindow.h"
#include <QTimer>

namespace Ui {
class weightwindow;
}

class weightwindow : public QMainWindow
{
    Q_OBJECT

public:
    weightwindow(User* us, QWidget *parent = nullptr);
    ~weightwindow();

private:
    Ui::weightwindow *ui;
    User* us = nullptr;
    WeightLoss* wei;
    QTimer timer;
    bool pause;
    ConfigWindow* confWin;
    datawindow* datWin;

private slots:
    void StartButton ();
    void PauseButton ();
    void StopButton ();
    void UiSample ();
    void OpenSessionButton ();
    void SerialConfigButton();
    void ReportButton();

};

#endif // WEIGHTWINDOW_H
