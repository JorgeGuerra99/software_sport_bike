//---------------------- CLASE BIKE--------------------------------------------------------------
//Contiene los diferentes sensores disponibles y el puerto serie de bike.
/**
 * @file bike.h
 * @brief Clase abstracta de bicicleta y sus derivadas
 * @version 1.0
 * @date
 * @authors Bazán María, Guerra Jorge
 * @title bicicleta
*/

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
/**
 * @brief The Bike class clase abstracta
 */
class Bike
{
protected:
    /**
     * @brief Bike constructor de la clase abstracta
     */
    Bike () { cout << "En constructor de bike "<< endl; };
    /**
     * @brief ~Bike destructor de la clase abstracta
     */
    virtual ~Bike () {cout << "En destructor de bike "<< endl; };
    virtual void ConfigSensors () = 0;
public:
    QSerialPortInfo portBikeInfo;
    QSerialPort portBike;
    virtual void ConfigSerial ( const QString &portname, const int &baud, const int &data, const int &par, const int &stop) = 0;
    virtual void ConfigSerial () = 0;
};


/**
 * @brief The StateBike class clase derivada de bike
 */
class StateBike: public Bike //Bicicleta fija
{
public:
    /**
     * @brief StateBike constructor de la clase derivada de bike para una bicicleta fija
     */
    StateBike ();
    /**
     * @brief ~StateBike destructor de la clase derivada de bike para bicicleta fija
     */
    virtual ~StateBike ();
    VelocitySensor < double > *vSensor;
    LoadSensor < double > *lSensor;
    PulseSensor < double > *pSensor;
    /**
     * @brief ConfigSerial configura el puerto serie de acuerdo a valores seleccionados por el usuario
     * @param portname
     * @param baud
     * @param data
     * @param par
     * @param stop
     */
    virtual void ConfigSerial (const QString &portname, const int &baud, const int &data, const int &par, const int &stop);
    /**
     * @brief ConfigSerial configura el puerto serie de acuerdo a valores predeterminados
     */
    virtual void ConfigSerial ();
    /**
     * @brief sensorsConfigured flag que establece si los sensores ya fueron configurados
     */
    bool sensorsConfigured = false;

private:
    /**
     * @brief ConfigSensors permite la configuración de los sensores que va a utilizar el puerto serie
     */
    virtual void ConfigSensors ();
};
#endif // BIKE_H
