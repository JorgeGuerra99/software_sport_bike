#ifndef SENSORS_H
#define SENSORS_H

#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QtSerialPort/QSerialPortInfo>
#include <math.h>

using namespace std;

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
    const T GetVeloc ( int &sel ) const { if ( sel == 1) return rpm; else return Sensors <T>::currentValue; }
    const T GetVelocMax () const { return velocMax; }
    const T GetVelocProm () const { return velocMed; }
private:
    T velocMax = 0;
    T velocMed;
    int dataRead = 0;
    T sumData = 0;
    T rpm;
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

template<class T>
Sensors<T>::Sensors(QSerialPort *p)
{
    port = p; //asocio direccion del puerto de bike a la direccion del puerto del sensor
    if (!port->isOpen())
    {
        if (!port->open(QSerialPort::ReadOnly))
        {
            cout << "Error al abrir el puerto serie" << endl;
        } else cout << "Puerto serie abierto con éxito" << endl;
    }
}
template<class T>
Sensors<T>::~Sensors()
{

}

template<class T>
PulseSensor<T>::PulseSensor(QSerialPort *p):Sensors<T> (p)
{
    cout << "En constructor de PulseSensor" << endl;
}

template<class T>
void PulseSensor<T>::GetValue()
{
    //aca poner como lee los datos del sensor y luego...
    T dataRead; //esto es lo que leería el sensor
    instantData.push_back(dataRead); //guardo dato instantáneo
    if (instantData.size() > 9)
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
    Sensors <T>::currentValue = sumData/(int) instantData.size();
    if (Sensors <T>::currentValue > maxFrequency)
    {
        maxFrequency = Sensors <T>::currentValue;
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
    //aca faltaría como lee del puerto serie
    T data;
    Sensors <T>::currentValue = data;
    dataRead++;
    sumData += Sensors <T>::currentValue;
    if (Sensors <T>::currentValue > velocMax) velocMax = Sensors <T>::currentValue;
    velocMed = sumData/dataRead;
    rpm = Sensors <T>::currentValue * 30 / (M_PI* 3.6* radius);
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
    if (Sensors <T>::currentValue > maxLoad)
    {
        maxLoad = Sensors <T>::currentValue;
    }

}

#endif // SENSORS_H
