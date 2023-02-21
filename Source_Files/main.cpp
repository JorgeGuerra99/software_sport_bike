#include <QApplication>
#include <Header_Files/mainwindow.h>
#include <Header_Files/configwindow.h>
#include <Header_Files/accesswindow.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    return 0;
}
