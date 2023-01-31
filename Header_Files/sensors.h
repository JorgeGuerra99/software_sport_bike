//---------------------------------CLASE SENSORS -----------------------------------------------------
// Template para que pueda ser utilizada dependiendo del tipo de dato que entrega cada sensor
// Este archivo contiene ademas las clases derivadas de la clase abstracta Sensors, donde cada clase derivada
// está adaptada de acuerdo a los diferentes tipos de sensores a utilizar

#ifndef SENSORS_H
#define SENSORS_H

#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QtSerialPort/QSerialPortInfo>
#include <math.h>
#include <unistd.h>

using namespace std;
enum {ERROR_SERIAL_OPEN};

template < class T >
class Sensors
{
protected:
    T currentValue = 0;
    Sensors ( QSerialPort *p);
    virtual ~Sensors ();
    virtual void GetValue () = 0;
public:
    QSerialPort *port; //de momento, se utiliza el objeto serialport de Qt
};

template < class T >
class PulseSensor: public Sensors < T >
{
public:
    PulseSensor (QSerialPort *p);
    virtual ~PulseSensor () {};
    virtual void GetValue ();
    void UpdateValue ();
    const T GetPulse () const { return Sensors <T>::currentValue; }
    const T GetFreqMax () const { return maxFrequency; }
private:
    T maxFrequency = 0;
    vector < T > instantData;
};

template < class T >
class VelocitySensor: public Sensors < T >
{
public:
    VelocitySensor (QSerialPort *p);
    virtual ~VelocitySensor () {};
    virtual void GetValue ();
    const T GetVeloc ( const int &sel = 0) const { if ( sel == 1) return kmh; else return this->currentValue; }
    const T GetVelocMax () const { return velocMax; }
    const T GetVelocProm () const { return velocMed; }
private:
    //currentValue almacena el valor en RPM
    T velocMax = 0;
    T velocMed;
    int dataRead = 0;
    T sumData = 0;
    T kmh;
    T radius = 0.02; //en metro
};

template < class T >
class LoadSensor: public Sensors < T >
{
public:
    LoadSensor (QSerialPort *p);
    virtual ~LoadSensor () {};
    virtual void GetValue ();
    const T GetLoad () const { return Sensors <T>::currentValue; }
    const T GetMaxLoad () const { return maxLoad; }
private:
    T maxLoad = 0;
};

// Definición de los métodos de la clase abstracta y las clases derivadas- Se incluyen en este archivo y no en cpp ya que se trabaja
// con templates

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
    // aca poner como lee los datos del sensor y luego...
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
