
#include <QApplication>
#include <Header_Files/mainwindow.h>
#include <Header_Files/configwindow.h>
#include <Header_Files/mainsession.h>
#include <Header_Files/accesswindow.h>
#include "Header_Files/session.h"
#include <unistd.h>
#include "Header_Files/bike.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
<<<<<<< HEAD

    return 0;
=======
    return 0; //
>>>>>>> c97b81c4ee28968a235d69d4e5a4866739a81a47
}
