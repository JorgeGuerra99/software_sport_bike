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
 * @brief The Bike class: Clase abstracta
 */
class Bike
{
public:
    /**
     * @brief Bike - Constructor
     */
    Bike () { cout << "En constructor de bike "<< endl; };
    virtual ~Bike () {cout << "En destructor de bike "<< endl; };
    /**
     * @brief ConfigSerial: Configura el puerto serie de acuerdo a valores seleccionados por el usuario
     * @param portname : Nombre del puerto serie
     * @param baud : Velocidad (BaudRate)
     * @param data : Bits de datos
     * @param par : Bit de paridad
     * @param stop : Bit de stop
     */
    virtual void ConfigSerial ( const QString &portname, const int &baud, const int &data, const int &par, const int &stop) = 0;
    /**
     * @brief ConfigSerial: Configura el puerto serie de acuerdo a valores predeterminados
     * @details portName "/dev/ttyACM0", baudRate 9600, data 8, noParity, stop 1
     */
    virtual void ConfigSerial () = 0;
    /**
     * @brief GetVelocitySensor: Permite obtener los datos del sensor de velocidad
     * @return Retorna un puntero del tipo VelocitySensor
     */
    virtual VelocitySensor <double>* GetVelocitySensor () const = 0;
    /**
     * @brief GetPulseSensor: Permite obtener los datos del sensor de pulso
     * @return Retorna un puntero del tipo PulseSensor
     */
    virtual PulseSensor <double>* GetPulseSensor () const = 0;
    /**
     * @brief GetLoadSensor: Permite obtener los datos del sensor de carga
     * @return Retorna un puntero del tipo LoadSensor
     */
    virtual LoadSensor <double>* GetLoadSensor () const = 0;
    /**
     * @brief GetPort: Permite hacer visible la propiedad portBike para ser utilizada por otra clase
     * @return Retorna un puntero al objeto del tipo QSerialPort
     */
    virtual const QSerialPort* GetPort () const = 0;
    /**
     * @brief GetPortInfo: Permimte hacer visible los puertos serie disponibles
     * @return Retorna un puntero al objeto del tipo QSerialPortInfo
     */
    virtual const QSerialPortInfo* GetPortInfo () const = 0;

protected:
    /**
     * @brief vSensor: Puntero del objeto sensor de velocidad utilizandolo como una propiedad del tipo double
     */
    VelocitySensor < double > *vSensor;
    /**
     * @brief lSensor: Puntero del objeto de sensor de carga utilizandolo como una propiedad del tipo double
     */
    LoadSensor < double > *lSensor;
    /**
     * @brief pSensor: Puntero del objeto de sensor de pulso utilizandolo como una propiedad del tipo double
     */
    PulseSensor < double > *pSensor;
    /**
     * @brief portBikeInfo: Contiene la información de los puertos serie disponibles
     */
    QSerialPortInfo portBikeInfo;
    /**
     * @brief portBike: Objeto del puerto serie a utilizar
     */
    QSerialPort portBike;
    /**
     * @brief ConfigSensors: Crea los objetos de los diferentes sensores y los asocia al puerto serie
     */
    virtual void ConfigSensors () = 0;
};


/**
 * @brief The StateBike class: Esta clase corresponde a una bicicleta estática
 * @details Clase derivada de bike
 */
class StateBike: public Bike
{
public:
    /**
     * @brief StateBike - Constructor
     */
    StateBike ();
    virtual ~StateBike ();
    /**
     * @brief ConfigSerial: Configura el puerto serie de acuerdo a valores seleccionados por el usuario
     * @param portname : Nombre del puerto serie
     * @param baud : Velocidad (BaudRate)
     * @param data : Bits de datos
     * @param par : Bit de paridad
     * @param stop : Bit de stop
     */
    virtual void ConfigSerial (const QString &portname, const int &baud, const int &data, const int &par, const int &stop);
    /**
     * @brief ConfigSerial: Configura el puerto serie de acuerdo a valores predeterminados
     * @details portName "/dev/ttyACM0", baudRate 9600, data 8, noParity, stop 1
     */
    virtual void ConfigSerial ();
    /**
     * @brief IsConfigured: Permite saber si los sensores están configurados permitiendo ver la propiedad privada de sensorsConfigured
     * @return Retorna un valor booleano donde si es verdadero ya se encuentran los sensores configurados
     */
    bool IsConfigured () const {return sensorsConfigured;}
    /**
     * @brief GetVelocitySensor
     * @return Puntero a sensor de velocidad
     */
    virtual VelocitySensor <double>* GetVelocitySensor () const { return vSensor;}
    /**
     * @brief GetPulseSensor
     * @return Puntero a sensor de pulso
     */
    virtual PulseSensor <double>* GetPulseSensor () const { return pSensor;};
    /**
     * @brief GetLoadSensor
     * @return Puntero a sensor de carga
     */
    virtual LoadSensor <double>* GetLoadSensor () const { return lSensor;};
    /**
     * @brief GetPort: Permite hacer visible la propiedad portBike para ser utilizada por otra clase
     * @return Retorna un puntero al objeto del tipo QSerialPort
     */
    virtual const QSerialPort* GetPort () const { return &portBike;}
    /**
     * @brief GetPortInfo: Permimte hacer visible los puertos serie disponibles
     * @return Retorna un puntero al objeto del tipo QSerialPortInfo
     */
    virtual const QSerialPortInfo* GetPortInfo () const { return &portBikeInfo;}
private:
    /**
     * @brief ConfigSensors: Crea los objetos de los diferentes sensores y los asocia al puerto serie
     */
    virtual void ConfigSensors ();
    /**
     * @brief sensorsConfigured
     * @note booleano que indica si los sensores ya fueron configurados
     */
    bool sensorsConfigured = false;
};
#endif // BIKE_H
