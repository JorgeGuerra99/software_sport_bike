//---------------------- CLASE BIKE--------------------------------------------------------------
//Contiene los diferentes sensores disponibles y el puerto serie de bike.

#ifndef BIKE_H
#define BIKE_H

#include <iostream>
#include <QList>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <string>
#include <list>
#include "sensors.h"

using namespace std;

class Bike
{
protected:
    Bike () { cout << "En constructor de bike "<< endl; };
    virtual ~Bike () {cout << "En destructor de bike "<< endl; };
    virtual void ConfigSensors () = 0;
public:
    QSerialPortInfo portBikeInfo;
    QSerialPort portBike;
    virtual void ConfigSerial ( const QString &portname, const int &baud, const int &data, const int &par, const int &stop) = 0;
    virtual void ConfigSerial () = 0;
};

class StateBike: public Bike //Bicicleta fija
{
public:
    StateBike ();
    virtual ~StateBike ();
    VelocitySensor < double > *vSensor;
    LoadSensor < double > *lSensor;
    PulseSensor < double > *pSensor;
    virtual void ConfigSerial (const QString &portname, const int &baud, const int &data, const int &par, const int &stop);
    virtual void ConfigSerial ();
    bool sensorsConfigured = false;
    //metodo para cargar config por defecto
    //metodo y ventana para configurar a mano
private:
    virtual void ConfigSensors ();
};
#endif // BIKE_H
