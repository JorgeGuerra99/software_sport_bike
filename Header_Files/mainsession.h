#ifndef MAINSESSION_H
#define MAINSESSION_H

#include <QWidget>
#include "user.h"
#include "session.h"
#include <QTime>

namespace Ui {
class MainSession;
}

class MainSession : public QWidget
{
    Q_OBJECT

public:
    explicit MainSession(QWidget *parent = nullptr);
    ~MainSession();
    int contSeg = 0;
    int contMin = 0;
    int contHours = 0;
    bool flagTimer = false;
public slots:
    void ActTimer ();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::MainSession *ui;
};

#endif // MAINSESSION_H
