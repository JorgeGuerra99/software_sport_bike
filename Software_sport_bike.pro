QT       += core gui
QT       += serialport
QT       += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Source_Files/accesswindow.cpp \
    Source_Files/bike.cpp \
    Source_Files/cardiowindow.cpp \
    Source_Files/configwindow.cpp \
    Source_Files/datawindow.cpp \
    Source_Files/dialogwin.cpp \
    Source_Files/main.cpp \
    Source_Files/mainwindow.cpp \
    Source_Files/qcustomplot.cpp \
    Source_Files/regwindow.cpp \
    Source_Files/session.cpp \
    Source_Files/user.cpp \
    Source_Files/weightwindow.cpp

HEADERS += \
    Header_Files/accesswindow.h \
    Header_Files/bike.h \
    Header_Files/cardiowindow.h \
    Header_Files/configwindow.h \
    Header_Files/datawindow.h \
    Header_Files/dialogwin.h \
    Header_Files/mainwindow.h \
    Header_Files/qcustomplot.h \
    Header_Files/regwindow.h \
    Header_Files/sensors.h \
    Header_Files/session.h \
    Header_Files/user.h \
    Header_Files/weightwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    UI_Files/accesswindow.ui \
    UI_Files/cardiowindow.ui \
    UI_Files/configwindow.ui \
    UI_Files/datawindow.ui \
    UI_Files/dialogwin.ui \
    UI_Files/mainwindow.ui \
    UI_Files/regwindow.ui \
    UI_Files/weightwindow.ui

DISTFILES += \
    5742154.png
