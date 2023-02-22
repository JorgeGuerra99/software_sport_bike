
/**
 * @file sensors.h
 * @brief Clase sensors y sus derivadas
 * @details Template para que pueda ser utilizada dependiendo del tipo de dato que entrega cada sensor
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef SENSORS_H
#define SENSORS_H

#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QtSerialPort/QSerialPortInfo>
#include <math.h>
#include <unistd.h>

using namespace std;
/**
 * @note ERROR_SERIAL_OPEN: Utilizado para contener errores al abrir el puerto serie
 */
enum {ERROR_SERIAL_OPEN};

template < class T >
/**
 * @brief The Sensors class
 */
class Sensors
{
protected:
    T currentValue = 0;
    /**
     * @brief Sensors
     * @param p : Objeto puerto serie asociado al sensor
     */
    Sensors ( QSerialPort *p);
    virtual ~Sensors ();
    virtual void GetValue () = 0;
public:
    QSerialPort *port;
};


template < class T >
/**
 * @brief The PulseSensor class
 */
class PulseSensor: public Sensors < T >
{
public:
    PulseSensor (QSerialPort *p);
    virtual ~PulseSensor () {};
    /**
     * @brief GetValue
     * @note Envía la orden al puerto serie y obtiene el dato respectivo
     */
    virtual void GetValue ();
    /**
     * @brief UpdateValue
     * @note Utilizado exclusivamente por el sensor de pulso. Actualiza el valor actual
     * de acuerdo a las últimas lecturas
     */
    void UpdateValue ();
    /**
     * @brief GetPulse
     * @return Valor actual de pulso
     */
    const T GetPulse () const { return Sensors <T>::currentValue; }
    /**
     * @brief GetFreqMax
     * @return Valor máximo de pulso leído
     */
    const T GetFreqMax () const { return maxFrequency; }
private:
    T maxFrequency = 0;
    /**
     * @brief instantData
     * @note Almacena datos instantáneos para luego promediar y actualizar currentValue
     */
    vector < T > instantData;
};

template < class T >
/**
 * @brief The VelocitySensor class
 */
class VelocitySensor: public Sensors < T >
{
public:
    VelocitySensor (QSerialPort *p);
    virtual ~VelocitySensor () {};
    /**
     * @brief GetValue
     * @note Actualiza valor actual del sensor
     */
    virtual void GetValue ();
    /**
     * @brief GetVeloc
     * @param sel : Si sel = 0 retorna velocidad RPM, si sel = 1 retorna velocidad en km/h
     * @return valor actual
     */
    const T GetVeloc ( const int &sel = 0) const { if ( sel == 1) return kmh; else return this->currentValue; }
    /**
     * @brief GetVelocMax
     * @return Velocidad máxima leída
     */
    const T GetVelocMax () const { return velocMax; }
    /**
     * @brief GetVelocProm
     * @return Velocidad promedio leída
     */
    const T GetVelocProm () const { return velocMed; }
private:
    //currentValue almacena el valor en RPM
    T velocMax = 0;
    T velocMed;
    /**
     * @brief dataRead
     * @note Cantidad de datos leídos. Utilizado para calcular la velocidad media
     */
    int dataRead = 0;
    /**
     * @brief sumData
     * @note Acumulador de datos
     */
    T sumData = 0;
    T kmh;
    /**
     * @brief radius
     * @note Valor de radio de la rueda de la bicicleta estática
     */
    T radius = 0.2; //en metro
};

template < class T >
class LoadSensor: public Sensors < T >
{
public:
    LoadSensor (QSerialPort *p);
    virtual ~LoadSensor () {};
    /**
     * @brief GetValue
     * @note Actualiza valor actual de carga
     */
    virtual void GetValue ();
    /**
     * @brief GetLoad
     * @return Valor actual de carga
     */
    const T GetLoad () const { return Sensors <T>::currentValue; }
    /**
     * @brief GetMaxLoad
     * @return Valor máximo registrado de carga
     */
    const T GetMaxLoad () const { return maxLoad; }
private:
    T maxLoad = 0;
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

/**
 * DEFINICIÓN DE MÉTODOS DE CLASE - INCLUIDOS EN HEADER FILE POR SER TEMPLATE
 */

template<class T>
Sensors<T>::Sensors(QSerialPort *p)
{
    port = p; //asocio direccion del puerto de bike a la direccion del puerto del sensor
    if (!port->isOpen()) //Si el puerto está cerrado
    {
        if (!port->open(QSerialPort::ReadWrite)) //Abro el puerto para lectura y escritura
        {
            cout << "Error al abrir el puerto serie" << endl;
            throw int (ERROR_SERIAL_OPEN);
        } else cout << "Puerto serie abierto con éxito" << endl;
    }
    cout << "constructor de sensors" << endl;
}
template<class T>
Sensors<T>::~Sensors()
{
    cout << "Destructor de sensors" << endl;
}


template<class T>
PulseSensor<T>::PulseSensor(QSerialPort *p):Sensors<T> (p)
{
    cout << "En constructor de PulseSensor" << endl;
}

template<class T>
void PulseSensor<T>::GetValue()
{
    QByteArray buf;
    char velC = 'P';
    if (this->port->isWritable())
    {
        this->port->write (&velC);
    }

    while(this->port->waitForReadyRead(100));
    buf = this->port->readAll();

    //    if (sizeof(T) == sizeof (double))
    //    {
            //consultar
    //    }

    this->port->clear();

    T dataRead; //esto es lo que leería el sensor

    dataRead = buf.toDouble();
    instantData.push_back(dataRead); //guardo dato instantáneo
    if (instantData.size() > 9) //Guardo hasta 10 lecturas de pulso instantáneo
    {
        UpdateValue();
        instantData.clear();
    }
 }

template<class T>
void PulseSensor<T>::UpdateValue()
{
    typedef typename std::vector<T>::iterator iterator;
    T sumData = 0;
    for (iterator it = instantData.begin(); it != instantData.end(); it ++)
    {
        sumData += (*it);
    }
    this->currentValue = sumData/(int) instantData.size(); //Promedio de los valores instantáneos
    if (this->currentValue > maxFrequency)
    {
        maxFrequency = this->currentValue; // Si supera el ultimo máximo guardado, actualizo máximo
    }
}

template<class T>
VelocitySensor<T>::VelocitySensor(QSerialPort *p): Sensors<T> (p)
{
    cout << "En constructor de VelocitySensor" << endl;
}

template<class T>
void VelocitySensor<T>::GetValue()
{
    char velC = 'V';
    QByteArray buf;
    if (this->port->isWritable())
    {
        this->port->write (&velC);
    }
    while(this->port->waitForReadyRead(100));
    buf = this->port->readAll();
    this->port->clear();
    T data;
    data = buf.toDouble();
    cout << "data velocidad = " << data << endl;
    this->currentValue = data;
    if (this->currentValue>0.0)
    {
       sumData += this->currentValue; // Acumulador
       if (this->currentValue > velocMax) velocMax = this->currentValue; //Valor maximo
       dataRead++;
       velocMed = sumData/dataRead; // Valor medio
    }
    kmh = this->currentValue * M_PI * 3.6 * radius / 30; //conversion a km/h
}


template<class T>
LoadSensor<T>::LoadSensor(QSerialPort *p): Sensors<T> (p)
{
    cout << "En constructor de LoadSensor" << endl;
}

template<class T>
void LoadSensor<T>::GetValue()
{
    //lectura del puerto y actualizo el current value
    QByteArray buf;

    char velC = 'L';
    if (this->port->isWritable())
    {
        this->port->write (&velC);
    }

    while(this->port->waitForReadyRead(100));
    buf = this->port->readAll();
    this->port->clear();
    T dataRead;
    dataRead = buf.toDouble();
    this->currentValue = dataRead;
    if (this->currentValue > maxLoad)
    {
        maxLoad = this->currentValue; // Valor maximo
    }

}

#endif // SENSORS_H
