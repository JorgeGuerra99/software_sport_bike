#include "Header_Files/bike.h"

StateBike::StateBike(): Bike ()
{
    cout << "En constructor de StateBike" << endl;
}

StateBike::~StateBike()
{
    if ( sensorsConfigured ){
        delete pSensor;
        delete vSensor;
        delete lSensor;
    }
    portBike.close();
    cout << "En destructor de StateBike" << endl;
}

void StateBike::ConfigSerial(const QString &portname, const int &baud, const int &data, const int &par, const int &stop)
{
    portBike.setPortName( portname );
    portBike.setBaudRate( ( QSerialPort::BaudRate ) baud);
    portBike.setDataBits( ( QSerialPort::DataBits ) data);
    portBike.setParity( ( QSerialPort::Parity ) par);
    portBike.setStopBits( ( QSerialPort::StopBits ) stop);
    ConfigSensors();
}

void StateBike::ConfigSerial()
{
    portBike.setPortName( "/dev/ttyACM0" );
    cout << "PUERTO "<< portBike.portName().toStdString() << endl;
    portBike.setBaudRate ( QSerialPort::Baud9600 );
    portBike.setDataBits ( QSerialPort::Data8 );
    portBike.setParity ( QSerialPort::NoParity );
    portBike.setStopBits ( QSerialPort::OneStop );
    portBike.setFlowControl( QSerialPort::NoFlowControl);
    ConfigSensors();
}

void StateBike::ConfigSensors()
{
    pSensor = new PulseSensor< double >( &portBike );
    vSensor = new VelocitySensor< double >( &portBike );
    lSensor = new LoadSensor< double >( &portBike );
    sensorsConfigured = true;
}
