#include "Header_Files/configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);

    ListPorts();
    ui->comboBox->addItems(portList);
    connect(ui->horizontalSlider, SIGNAL (valueChanged(int)), this, SLOT(BaudRateChanged(int)));
    connect (ui->pushButton, SIGNAL (clicked()), this, SLOT (SaveConfig()));
    connect(ui->pushButton_2, SIGNAL (clicked()), this, SLOT (Cancel()));
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::ListPorts()
{
    QList <QSerialPortInfo> ports = myBike.portBikeInfo.availablePorts();
    portList.resize(ports.size());
    int i = 0;
    if (!ports.empty())
    {
        for (QList <QSerialPortInfo>::iterator it = ports.begin(); it != ports.end(); it ++, i++)
        {
            portList[i] = (*it).portName();
        }
    }
}

void ConfigWindow::BaudRateChanged(int value)
{
    switch (value) {
    case 0:
        ui->label_4->setText("4800");
        baud = QSerialPort::Baud4800;
        break;
    case 1:
        ui->label_4->setText("9600");
        baud = QSerialPort::Baud9600;
        break;
    case 2:
        ui->label_4->setText("19200");
        baud = QSerialPort::Baud19200;
        break;
    case 3:
        ui->label_4->setText("38400");
        baud = QSerialPort::Baud38400;
        break;
    default:
        break;
    }
}

void ConfigWindow::SaveConfig()
{
    int wpar, wdata, wstop;
    if (ui->radioButton->isChecked()) wpar = QSerialPort::NoParity;
    else if (ui->radioButton_2->isChecked()) wpar = QSerialPort::EvenParity;
    else if (ui->radioButton_3->isChecked()) wpar = QSerialPort::OddParity;

    wdata = ui->comboBox_2->currentIndex() + 5;

    wstop = ui->comboBox_3->currentIndex() + 1;

    myBike.ConfigSerial(ui->comboBox->currentText(), baud, wdata, wpar, wstop);

    cout << "Puerto seleccionado: " << myBike.portBike.portName().toStdString() << endl;
    cout << "BaudRate: " << myBike.portBike.baudRate() << endl;
    cout << "Paridad: " << myBike.portBike.parity() << endl;
    cout << "Stop: " << myBike.portBike.stopBits() << endl;
    cout << "Data: " << myBike.portBike.dataBits() << endl;
    this->close();
}
