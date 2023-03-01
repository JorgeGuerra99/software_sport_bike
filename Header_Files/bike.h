/**
 * @file bike.h
 * @brief Contiene los diferentes sensores disponibles y el puerto serie de bike.
 * @date 2023
 * @authors Bazán María, Guerra Jorge
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
 * @brief The Bike class
 * @note Clase abstracta
 * @
 */
class Bike
{
public:
    Bike () { cout << "En constructor de bike "<< endl; };
    virtual ~Bike () {cout << "En destructor de bike "<< endl; };
    virtual void ConfigSerial ( const QString &portname, const int &baud, const int &data, const int &par, const int &stop) = 0;
    virtual void ConfigSerial () = 0;
    VelocitySensor < double > *vSensor;
    LoadSensor < double > *lSensor;
    PulseSensor < double > *pSensor;
    /**
     * @brief portBikeInfo
     * @note Contiene la información de los puertos serie disponibles
     */
    QSerialPortInfo portBikeInfo;
    /**
     * @brief portBike
     * @note Objeto del puerto serie a utilizar
     */
    QSerialPort portBike;

protected:
    virtual void ConfigSensors () = 0;


};


/**
 * @brief The StateBike class
 * @note Clase derivada de bike
 * @details Esta clase corresponde a una bicicleta estática
 */
class StateBike: public Bike
{
public:
    StateBike ();
    virtual ~StateBike ();
    /**
     * @brief ConfigSerial
     * @note configura el puerto serie de acuerdo a valores seleccionados por el usuario
     * @param portname : Nombre del puerto serie
     * @param baud : Velocidad (BaudRate)
     * @param data : Bits de datos
     * @param par : Bit de paridad
     * @param stop : Bit de stop
     */
    virtual void ConfigSerial (const QString &portname, const int &baud, const int &data, const int &par, const int &stop);
    /**
     * @brief ConfigSerial
     * @note configura el puerto serie de acuerdo a valores predeterminados
     * @details portName "/dev/ttyACM0", baudRate 9600, data 8, noParity, stop 1
     */
    virtual void ConfigSerial ();
    /**
     * @brief ConfigSensors
     * @note crea los objetos de los diferentes sensores y los asocia al puerto serie
     */
    bool IsConfigured () const {return sensorsConfigured;}
private:
    virtual void ConfigSensors ();
    /**
     * @brief sensorsConfigured
     * @note flag que indica si los sensores ya fueron configurados
     */
    bool sensorsConfigured = false;
};
#endif // BIKE_H
