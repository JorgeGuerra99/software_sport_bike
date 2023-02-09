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

class CardioWindow : public QMainWindow
{
    Q_OBJECT

public:
     CardioWindow(User*& , QWidget *parent = nullptr);
    ~CardioWindow();

private:
    Ui::CardioWindow *ui;
    User* us = nullptr;
    Cardio* car;
    QTimer timer;
    bool pause;
    ConfigWindow* confWin;
    datawindow* datWin;

private slots:
    void StartButton();
    void PauseButton();
    void StopButton();
    void UiSample ();
    void ExportDataButton ();
    void OpenSessionButton ();
    void SerialConfigButton ();
    void ReportButton ();
};

#endif // CARDIOWINDOW_H
