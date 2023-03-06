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
 * @brief enum: Utilizado para contener errores de diferentes tipos
 * @details Errores al abrir el puerto serie
 */
enum {ERROR_SERIAL_OPEN};

template < class T >
/**
 * @brief The Sensors class: Clase abstracta para sensor genérico
 */
class Sensors
{
protected:
    /**
     * @brief Sensors - Constructor
     * @param p: Objeto puerto serie asociado al sensor
     */
    Sensors ( QSerialPort *p);
    virtual ~Sensors ();
    /**
     * @brief currentValue: Valor actual que presenta el sensor
     */
    T currentValue = 0;
    /**
     * @brief port: Puntero del puerto serie
     */
    QSerialPort *port;
public:
    /**
     * @brief GetValue: Envía la orden al puerto serie y obtiene el dato respectivo
     */
    virtual void GetValue () = 0;
};


template < class T >
/**
 * @brief The PulseSensor class: Clase derivada de sensor para sensor de pulso
 */
class PulseSensor: public Sensors < T >
{
public:
    /**
     * @brief PulseSensor - Constructor
     * @param p: Objeto puerto serie asociado al sensor
     */
    PulseSensor (QSerialPort *p);
    virtual ~PulseSensor () { cout << "Destructor de PulseSensor" << endl; };
    /**
     * @brief GetValue: Envía la orden al puerto serie y obtiene el dato respectivo
     */
    virtual void GetValue ();
    /**
     * @brief GetPulse: Obtiene el dato respectivo del sensor
     * @return Retorna el valor actual de pulso
     */
    const T GetPulse () const { return Sensors <T>::currentValue; }
    /**
     * @brief GetFreqMax: Obtiene el dato respectivo a la máxima frecuencia
     * @return Retorna el valor máximo de pulso leído
     */
    const T GetFreqMax () const { return maxFrequency; }
private:
    /**
     * @brief maxFrequency: Propiedad donde se almacena la máxima frecuencia
     */
    T maxFrequency = 0;
    /**
     * @brief instantData: Almacena datos instantáneos para luego promediar y actualizar currentValue
     */
    vector < T > instantData;
    /**
     * @brief UpdateValue: Utilizado exclusivamente por el sensor de pulso. Actualiza el valor actual de acuerdo a las últimas lecturas
     */
    void UpdateValue ();
};

template < class T >
/**
 * @brief The VelocitySensor class: Clase derivada de sensor para sensor de velocidad
 */
class VelocitySensor: public Sensors < T >
{
public:
    /**
     * @brief VelocitySensor - Constructor
     * @param p: Objeto puerto serie asociado al sensor
     */
    VelocitySensor (QSerialPort *p);
    virtual ~VelocitySensor () { cout << "Destructor de VelocitySensor" << endl; };
    /**
     * @brief GetValue: Actualiza valor actual del sensor
     */
    virtual void GetValue ();
    /**
     * @brief GetVeloc: Permite obtener la velocidad del sensor en RPM o km/h de acuerdo al parámetro sel
     * @param sel: Si sel = 0 retorna velocidad RPM, si sel = 1 retorna velocidad en km/h
     * @return Retorna el valor actual en la unidad seleccionada
     */
    const T GetVeloc ( const int &sel = 0) const { if ( sel == 1) return kmh; else return this->currentValue; }
    /**
     * @brief GetVelocMax: Permite obtener el valor de la velocidad máxima sensada
     * @return Retorna la velocidad máxima leída
     */
    const T GetVelocMax () const { return velocMax; }
    /**
     * @brief GetVelocProm: Permite obtener el valor promedio de los datos sensados
     * @return Retorna la velocidad promedio leída
     */
    const T GetVelocProm () const { return velocMed; }
private:
    /**
     * @brief velocMax: Propiedad donde se almacena la velocidad máxima
     */
    T velocMax = 0;
    /**
     * @brief velocMed: Propiedad donde se almacena la velocidad media o promedio
     */
    T velocMed;
    /**
     * @brief dataRead: Cantidad de datos leídos.
     * @details Utilizado para calcular la velocidad media
     */
    int dataRead = 0;
    /**
     * @brief sumData: Acumulador de datos
     */
    T sumData = 0;
    /**
     * @brief kmh: Propiedad donde se almacena la velocidad cuya unidades son km/h
     */
    T kmh;
    /**
     * @brief radius: Valor de radio de la rueda de la bicicleta estática
     * @details el valor en el que está instanciado es en metros
     */
    T radius = 0.2; //en metro
};

template < class T >
/**
 * @brief The LoadSensor class: Clase derivada de sensor para sensor de carga
 */
class LoadSensor: public Sensors < T >
{
public:
    /**
     * @brief LoadSensor - Constructor
     * @param p: Objeto puerto serie asociado al sensor
     */
    LoadSensor (QSerialPort *p);
    virtual ~LoadSensor () { cout << "Destructor de LoadSensor" << endl;};
    /**
     * @brief GetValue: Actualiza valor actual de carga
     */
    virtual void GetValue ();
    /**
     * @brief GetLoad: Permite obtener el valor actual del sensor
     * @return Retorna el valor actual de carga
     */
    const T GetLoad () const { return Sensors <T>::currentValue; }
    /**
     * @brief GetMaxLoad: Permite obtener el máximo de valor de carga usado durante la sesión
     * @return Retorna el valor máximo registrado de carga
     */
    const T GetMaxLoad () const { return maxLoad; }
private:
    /**
     * @brief maxLoad: Propiedad donde se almacena el valor de la máxima carga sensada
     */
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
