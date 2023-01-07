
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
//    MainWindow w;
//    w.show();
//    return a.exec();


    Cardio car (23, 'M');
    car.Start();

    for (int i=0; i<1000; i++)
    {
        car.Sample();
        sleep (1);
    }

    return 0;
}
