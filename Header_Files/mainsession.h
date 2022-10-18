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
private:
    Ui::MainSession *ui;
};

#endif // MAINSESSION_H
