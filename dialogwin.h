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
    explicit DialogWin(string msj, QWidget *parent = nullptr);
    ~DialogWin();

private:
    Ui::DialogWin *ui;
};

#endif // DIALOGWIN_H
