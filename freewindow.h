#ifndef FREEWINDOW_H
#define FREEWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Header_Files/session.h"
#include "Header_Files/user.h"
#include "Header_Files/configwindow.h"
#include "Header_Files/datawindow.h"

namespace Ui {
class FreeWindow;
}

class FreeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FreeWindow(User*&, QWidget *parent = nullptr);
    ~FreeWindow();

private:
    Ui::FreeWindow *ui;
    Free* fre;
    User* us = nullptr;
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

#endif // FREEWINDOW_H
