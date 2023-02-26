#include "Header_Files/session.h"

//-----------------------------------------------------------------------------------------
//------------------------------- MÉTODOS DE SESSION --------------------------------------
//-----------------------------------------------------------------------------------------

double Session::GetLastData(int a) const
{
    switch (a) {
    case 0:
        return pulseData.back();
    case 1:
        return velocData.back();
    case 2:
        return  dataOfLoad.back();
    default:
        return 0;
    }
}

const vector<double> Session::GetAllData(char sel) const
{
    switch (sel) {
    case 'P':
        return pulseData;
    case 'V':
        return velocData;
    case 'L':
        return dataOfLoad;
    default:
        return pulseData;
    }
}

Session::Session(const string& name, const int& age, const char& sex, const float& weight, const float& height)
{
    dataUser.name = name;
    dataUser.age = age;
    dataUser.sex = sex;
    dataUser.weight = weight;
    dataUser.height = height;

    cout << "En constructor de session" << endl;
}

double Session::GetVelocMaxMed(const int &s)
{
    if (s == 0) return velMax;
    else if (s == 1) return velMed;
    else return -1;
}

//-----------------------------------------------------------------------------------------
//------------------------------- MÉTODOS DE CARDIO ---------------------------------------
//-----------------------------------------------------------------------------------------

Cardio::Cardio(const string& name, const int& age, const char& sex, const float& weight, const float& height): Session (name, age, sex, weight, height)
{
    sessionType = "Cardio";
    time_t now;
    time (&now);
    char *c = ctime (&now);
    string localDate (c);
    date = localDate;
    cout << "En constructor de cardio" << endl;
}

//------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
void Cardio::Start()
{
    try {
        sesAct = true;
        LoadConfig(); //cargo configuraciones del archivo
        distance = 0.0;
        if (!bike.sensorsConfigured) bike.ConfigSerial(); //Configura el puerto serie
        screenMessage = "Entrenamiento iniciado";
    }  catch (int e) {
        if (e == ERROR_SERIAL_OPEN)
        {
            cout << "ERROR SERIAL" << endl;
            sesAct = false; //No inicia si no puede abrir el puerto serie
        }
    }
}

//----------------------------------------------------------------------------------------------------------------
bool Cardio::Pause()
{
    if (velocData.size()>5)
    {
        double auxData = 0.0;
        for (vector<double>::iterator it = velocData.end()-1; it!= velocData.end()-5; it--)
        {
            auxData+= *it;
        }
        if (auxData == 0.0)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------
void Cardio::End()
{

    sesAct = false;
    paused = false;
}

//---------------------------------------------------------------------------------------------------------
void Cardio::WriteReport() const
{
    fstream sessionFile;
    string fileName;
    fileName =  sessionType + string ("_") + dataUser.name + string ("_") + date;
    fileName.pop_back();
    sessionFile.open(fileName, ios::app);
    sessionFile << *this;
}
//-----------------------------------------------------------------------------------------------------------
void Cardio::ReadReport()
{
    fstream sessionFile;
    string fileName;
    fileName =  sessionType + string ("_") + dataUser.name + string ("_") + date;
    sessionFile.open( fileName, ios::in);
    if (!sessionFile) cout << "ERROR AL ABRIR EL ARCHIVO" << endl;
    sessionFile >> *this;
}

double Cardio::CalcCalories() const

{
    double cal = 0.0;
    double ind1 = 0.049;
    double indWeig = 2.2;
    double ind2 = 0.071;
    static int intLow=0, intHigh=0;
    if (velocData.back()<=70) intLow++;
    else intHigh++;
    cal = ind1 * ( dataUser.weight * indWeig ) * ( intLow * 0.016667 );
    cal += ind2 * ( dataUser.weight * indWeig ) * ( intHigh * 0.016667 );
    return cal;
}

//-------------------------------------------------------------------------------------------------------------
void Cardio::Sample()
{
    if (sesAct == true)
    {
        //una forma de usar esta función es que en la clase haya un objeto timer interno que se inicialice con
        //el método start - Encontré en internet el QTimer pero no se si usar ese

        timeSes++;
        //Actualizo datos de sensores
        bike.vSensor->GetValue();
        bike.pSensor->GetValue();
        bike.lSensor->GetValue();

        //Obtengo valores y guardo en vector
        velocData.push_back(bike.vSensor->GetVeloc());
        pulseData.push_back(bike.pSensor->GetPulse());
        dataOfLoad.push_back(bike.lSensor->GetLoad());
        distance+= bike.vSensor->GetVeloc(1)*sampleTime/3600; //obtengo distancia en km

        velMax = bike.vSensor->GetVelocMax();
        velMed = bike.vSensor->GetVelocProm();

        //obtengo las calorias
        if(velocData.back()!=0.0)
        {
            calories = CalcCalories();
        }

        //Evalúo etapa actual
        StageEval(timeSes);
        if (timeSes > 10 and !NoRutAlm()) screenMessage = "Va a buen ritmo";
        //Evaluo velocidad en un rango de variación del 10%
        if (Pause())
        {
            cout << "Entrenamiento pausado" << endl;
            sesAct = false;
            paused = true;
        }

        //Evalúo PPM
        if(AlarmPpm())
        {
            screenMessage = "Frecuencia cardíaca alta";
        }
    }
    if (paused)
    {
        bike.vSensor->GetValue();
        screenMessage = "Entrenamiento pausado";
        if (bike.vSensor->GetVeloc()!= 0.0)
        {
            paused = false;
            sesAct = true;
        }
    }
}
//_-------------------------------------------------------------------------------------------------------------
bool Cardio::NoRutAlm()
{
    if (velocData.back() > velocRef[stage]+ velocRef[stage]*0.1)
    {
        screenMessage = "Reduzca la velocidad";
        return true;
    }
    if (velocData.back() < velocRef[stage] - velocRef[stage]*0.1)
    {
        screenMessage = "Incremente la velocidad";
        return true;
    }
    cout << "va a ritmo" << endl;
    return false;
}
//------------------------------------------------------------------------------------------------------------------
void Cardio::StageEval(const int &tim)
{
    for (int i=0; i < (int) timeRef.size(); i++)
    {
        if ((int) tim/sampleTime > timeRef[i])
        {
            stage = i+1;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
void Cardio::LoadConfig()
{
    //---------------------------------------------------------------------------------------------------------------------
    //Este método carga los valores de los vectores de timeRef y velocRef (de referencia para la sesión de entrenamiento)
    //---------------------------------------------------------------------------------------------------------------------

    fstream configFile;
    int timePos = 0, velocPos = 0; //posiciones dentro del archivo
    int timeAux, velocAux;

    // Apertura de archivo
    configFile.open("Session1Config.txt", ios::in);
    if (!configFile)
    {
        screenMessage = "Error al abrir el archivo de configuración";
        throw (int (FILE_CONFIG_ERROR));
    }

    //Extraigo posiciones de acuerdo a marcadores preestablecidos
    string line;
    while (!(configFile.eof()))
    {
        getline (configFile,line);
        if (line == "TIME_REF")
        {
            timePos = configFile.tellg();
        } else if (line == "VELOC_REF")
        {
            velocPos = configFile.tellg();
        }
    }

    configFile.clear(); //para borrar los bits de error (eof)
    configFile.seekg(timePos); //me posiciono en el comienzo de los datos de tiempo
    while (configFile >> timeAux)
    {
        timeRef.push_back(timeAux); //lleno vector
    }
    configFile.clear();
    configFile.seekg(velocPos); //me posiciono en los datos de velocidad
    while (configFile >> velocAux)
    {
        velocRef.push_back(velocAux); //lleno vector
    }

    //-------------Lineas para mostrar los datos que se cargan - solo para pruebas --------------------

//    cout << "Datos cargados de tiempo: " << endl;
//    for (int i = 0; i < (int)timeRef.size(); i++)
//    {
//        cout << timeRef [i] << endl;
//    }
//    cout << "Datos cargados de velocidad: " << endl;
//    for (int i = 0; i < (int)velocRef.size(); i++)
//    {
//        cout << velocRef [i] << endl;
//    }
}

//------------------------------------------------------------------------------------------------------
bool Cardio::AlarmPpm()
{
    float freqMaxRef;
    if(dataUser.sex=='M')
    {
        freqMaxRef = 180 + ( 0.5 * dataUser.age );
    } else
    {
        freqMaxRef = 210.7 - ( 0.8 * dataUser.age );
    }
    if (pulseData.back() > freqMaxRef)
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------------
ostream& operator<< (ostream& ios, const Cardio& car)
{
    ios << "-----ENTRENAMIENTO:CARDIO-----" << endl;
    ios << "Usuario: " << car.dataUser.name << endl;
    ios << "DATOS DE ENTRENAMIENTO: " << endl;
    ios << "Fecha: " << car.date << endl;
    ios << "Tiempo: " << car.timeSes << endl;
    ios << "Velocidad máxima: " << car.bike.vSensor->GetVelocMax() << endl;
    ios << "Velocidad promedio: " << car.bike.vSensor->GetVelocProm() << endl;
    ios << "Distancia estimada: " << car.distance << endl;
    ios << endl << endl << endl;
    ios << "Datos instantáneos:" << endl;
    ios << "VELOCIDAD" <<endl;
    for (int i = 0; i < (int) car.velocData.size(); i++)
    {
        ios << car.velocData [i] << endl;
    }
    ios << "PPM" <<endl;
    for (int i = 0; i < (int) car.pulseData.size(); i++)
    {
        ios << car.pulseData [i] << endl;
    }
    ios << "CARGA" <<endl;
    for (int i = 0; i < (int) car.dataOfLoad.size(); i++)
    {
        ios << car.dataOfLoad [i] << endl;
    }
    ios << "FIN_DATOS" << endl;
    return ios;
}
//--------------------------------------------------------------------------------------------------------
istream& operator>> (istream& ist, Cardio& car)
{
    //---------------OPERADOR >> PARA SESSION CARDIO -------------------------------------------
    // Permite armar el stream de entrada con los datos de entrenamiento de una sesión cardio y cargarlos en un objeto cardio

    string line, aux;
    while (line.find("Usuario") == string::npos )
    {
        getline (ist, line);
    }
    if (line.substr(line.find_first_of(" ")+1) != car.dataUser.name)
    {
        //Si el usuario del archivo no es coincidente con el usuario que inició sesión
        throw int (INVALID_USER);
        return ist;
    }
    std::setlocale(LC_NUMERIC,"C"); //Con esta linea permite reconocer al "." de los strings como delimitador de punto flotante
                                    //de lo contrario, tomaba "," entonces recortaba los números a su parte entera

    //Extracción de datos

    while (line.find("Datos instantáneos") == string::npos)
    {
        getline (ist,line);
        if (line.find("Tiempo") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            car.timeSes = stoi (aux);
            cout << "Tiempo = " << car.timeSes << endl;
        }
        if (line.find("Velocidad máxima") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            car.velMax = stod (aux);
            cout << "Velocidad máxima: " << car.velMax << endl;
        }
        if (line.find("Velocidad promedio") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            car.velMed = atof (aux.c_str());
            cout << "Velocidad promedio: " << car.velMed << endl;
        }
        if (line.find("Distancia") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            car.distance = stof (aux);
            cout << "Distancia estimada: " << car.distance << endl;
        }
    }
    //Extracción de datos instantáneos: Llenado de los vectores con los datos instantáneos
    getline (ist,line);
    while (!ist.eof())
    {
        if (line.find("VELOCIDAD") != string::npos)
        {
            getline (ist,line);
            while (line.find("PPM") == string::npos)
            {
                car.velocData.push_back(stod (line));
                getline (ist, line);
            }
        } else if (line.find("PPM") != string::npos){
            getline (ist,line);
            while (line.find("CARGA") == string::npos)
            {
                car.pulseData.push_back(stod (line));
                getline (ist, line);
            }
        } else if (line.find("CARGA") != string::npos)
        {
            getline (ist,line);
            while (!ist.eof())
            {
                car.dataOfLoad.push_back(stod (line));
                getline (ist, line);
            }
        }
    }

    // Líneas para mostrar los datos cargados, comentar si es necesario...

//    cout << "Datos de velocidad cargados: " << endl;
//    for (int i = 0; i < (int) car.velocData.size(); i++)
//    {
//        cout << car.velocData[i] << endl;
//    }
//    cout << "Datos de pulso cargados: " << endl;
//    for (int i = 0; i < (int) car.pulseData.size(); i++)
//    {
//        cout << car.pulseData[i] << endl;
//    }
//    cout << "Datos de carga cargados: " << endl;
//    for (int i = 0; i < (int) car.dataOfLoad.size(); i++)
//    {
//        cout << car.dataOfLoad[i] << endl;
//    }
    return ist;
}

//---------------------------------------------------------------------------------------------------------------------------------
//------------------------------ MÉTODOS DE WEIGHTLOSS ----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

WeightLoss::WeightLoss (const string& name, const int& age, const char& sex, const float& weight, const float& height): Session (name, age, sex, weight, height)
{
    sessionType = "Weightloss";
    //Se obtiene de forma automática la fecha la fecha en la que el usuario realiza la sesión
    time_t now;
    time (&now);
    char *c = ctime (&now);
    string localDate (c);
    date = localDate;
    cout << "En constructor de weightloss" << endl;
}
//--------------------------------------------------------------------------------------------------------
void WeightLoss::Start ()
{
    try {
        sesAct = true;
        LoadConfig(); //cargo configuraciones del archivo
        distance = 0.0;
        if (!bike.sensorsConfigured) bike.ConfigSerial();
        screenMessage = "Entrenamiento iniciado";
        cout << "Sesión iniciada" << endl;
    }  catch (int e) {
        if (e == ERROR_SERIAL_OPEN)
        {
            cout << "ERROR SERIAL" << endl;
            sesAct = false;
        }
    }
}

bool WeightLoss::Pause ()
{
    //Se evalua los últimos 5 valores de velocidad, si la suma de los mismos es cero retorna true para pausar el entrenamiento automáticamente
    if (velocData.size()>5)
    {
        double auxData = 0.0;
        for (vector<double>::iterator it = velocData.end()-1; it!= velocData.end()-5; it--)
        {
            auxData+= *it;
        }
        if (auxData == 0.0)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------
void WeightLoss::End ()
{
    //Se desactiva las variables para evitar que siga realizando el muestreo de datos
    sesAct = false;
    paused = false;
}
//--------------------------------------------------------------------------------------------------------
bool WeightLoss::NoRutAlm ()
{
    //---------------------------------------------------------------------------------------------------------
    //Evalúa si la velocidad actual se mantiene dentro del rango de pulsaciones definido para la rutrina
    //---------------------------------------------------------------------------------------------------------
    if(pulseData.back() > intensityMaxFc)
    {
        screenMessage = "Reduzca la velocidad";
        return true;
    }
    if(pulseData.back() < intensityMinFc)
    {
        screenMessage = "Incremente la velocidad";
        return true;
    }
    cout << "va a ritmo" << endl;
    return false;
}
//--------------------------------------------------------------------------------------------------------
void WeightLoss::Sample ()
{
    //-----------------------------MÉTODO DE MUESTREO A EJECUTARSE REITERADAMENTE DURANTE LA SESIÓN ----------------------------
    if (sesAct == true)
    {
        //operaciones de muestreo de datos

        timeSes++;
        //Actualizo datos de sensores
        bike.vSensor->GetValue();
        bike.pSensor->GetValue();
        bike.lSensor->GetValue();

        //Obtengo valores y guardo en vector
        velocData.push_back(bike.vSensor->GetVeloc());
        pulseData.push_back(bike.pSensor->GetPulse());
        dataOfLoad.push_back(bike.lSensor->GetLoad());
        distance+= bike.vSensor->GetVeloc(1)*sampleTime/3600; //obtengo distancia en km

        velMax = bike.vSensor->GetVelocMax();
        velMed = bike.vSensor->GetVelocProm();

        //obtengo el calculo de calorias
        calories = CalcCalories();

        //Evalúo el tiempo que va transcurriendo
        if (sampleTime == timeRef)
        {
            End();
        }
        if (timeSes > 10 and !NoRutAlm()) screenMessage = "Va a buen ritmo";

        //Evaluo velocidad en un rango de variación del 10%
        if (Pause())
        {
            cout << "Entrenamiento pausado" << endl;
            sesAct = false;
            paused = true;
        }

        //Evalúo PPM
        if(AlarmPpm())
        {
            screenMessage = "Frecuencia cardíaca alta";
        }

    }
    if (paused)
    {
        bike.vSensor->GetValue();
        screenMessage = "Entrenamiento pausado";
        if (bike.vSensor->GetVeloc()!= 0.0)
        {
            paused = false;
            sesAct = true;
        }
    }
}
//--------------------------------------------------------------------------------------------------------
void WeightLoss::LoadConfig ()
{
    //---------------------------------------------------------------------------------------------------------------------
    //Este método carga los valores de timeRef e intensidad (de referencia para la sesión de entrenamiento)
    //---------------------------------------------------------------------------------------------------------------------

    fstream configFile;
    int timePos = 0, intePos = 0, contInt = 0; //posiciones dentro del archivo
    int timeAux;
    float inteAux;

    // Apertura de archivo
    configFile.open("Session2Config.txt", ios::in);
    if (!configFile)
    {
        screenMessage = "Error al abrir el archivo de configuración";
        throw (int (FILE_CONFIG_ERROR));
    }

    //Extraigo posiciones de acuerdo a marcadores preestablecidos
    string line;
    while (!(configFile.eof()))
    {

        getline (configFile,line);
        if (line == "TIME_REF")
        {
            timePos = configFile.tellg();
        } else if (line == "INTENSITY")
        {
            intePos = configFile.tellg();
        }
    }

    configFile.clear(); //para borrar los bits de error (eof)
    configFile.seekg(timePos); //me posiciono en el comienzo de los datos de tiempo
    while (configFile >> timeAux)
    {
        timeRef = timeAux;
    }
    configFile.clear();
    configFile.seekg(intePos); //me posiciono en los datos de intensidad
    while (configFile >> inteAux)
    {
        if ( contInt == 0)
        {
            intensityMinFc = ( inteAux * ( 220 - dataUser.age ) ) / 100;
            contInt++;
        }
        else
        {
            intensityMaxFc = ( inteAux * ( 220 - dataUser.age ) ) / 100;
            contInt = 0;
        }
    }

   //-------------Lineas para mostrar los datos que se cargan - solo para pruebas --------------------
/*
    cout << "Datos cargados de tiempo: " << endl;
    cout << timeRef << endl;
    cout << "Datos cargados de intensidad minima y maxima: " << endl;
    cout << intensityMinFc << endl;
    cout << intensityMaxFc << endl;
    */
}
//--------------------------------------------------------------------------------------------------------
void WeightLoss::WriteReport () const
{
    //este método permite exportar unicamente la sesión realizada
    fstream sessionFile;
    string fileName;
    //definición del nombre de archivo en el que se guarda la sesión en formato "fecha-usuario.txt"
    fileName =  sessionType + string ("_") + dataUser.name + string ("_") + date;
    // apertura del archivo de texto para posterior guardado de los datos de la sesión
    sessionFile.open(fileName, ios::app);
    sessionFile << *this;
}
//--------------------------------------------------------------------------------------------------------

void WeightLoss::ReadReport()
{
    string fileName;
    fileName =  sessionType + string ("_") + dataUser.name + string ("_") + date;
    fstream sessionFile;
    sessionFile.open(fileName, ios::in);
    if (!sessionFile) cout << "ERROR AL ABRIR EL ARCHIVO" << endl;
    sessionFile >> *this;
}

//--------------------------------------------------------------------------------------------------------
bool WeightLoss::AlarmPpm ()
{
    float freqMaxRef;
    if(dataUser.sex=='M')
    {
        freqMaxRef = 180 + ( 0.5 * dataUser.age );
    } else
    {
        freqMaxRef = 210.7 - ( 0.8 * dataUser.age );
    }
    if (pulseData.back() > freqMaxRef)
    {
        return true;
    }
    return false;
}

double WeightLoss::CalcCalories ( ) const
{
    double cal = 0.0;
    double ind1 = 0.049;
    double indWeig = 2.2;
    double ind2 = 0.071;
    static int intLow=0, intHigh=0;
    if (velocData.back()<=70) intLow++;
    else intHigh++;
    cal = ind1 * ( dataUser.weight * indWeig ) * ( intLow * 0.016667 );
    cal += ind2 * ( dataUser.weight * indWeig ) * ( intHigh * 0.016667 );
    return cal;
}

istream& operator>> (istream& ist, WeightLoss& wei)
{
    //---------------OPERADOR >> PARA SESSION WEIGHTLOSS -------------------------------------------
    // Permite armar el stream de entrada con los datos de entrenamiento de una sesión weightloss y cargarlos en un objeto weightloss

    string line, aux;
    while (line.find("Usuario") == string::npos )
    {
        getline (ist, line);
    }
    if (line.substr(line.find_first_of(" ")+1) != wei.dataUser.name)
    {
        //Si el usuario del archivo no es coincidente con el usuario que inició sesión
        throw int (INVALID_USER);
        return ist;
    }
    std::setlocale(LC_NUMERIC,"C"); //Con esta linea permite reconocer al "." de los strings como delimitador de punto flotante
                                    //de lo contrario, tomaba "," entonces recortaba los números a su parte entera

    //Extracción de datos

    while (line.find("Datos instantáneos") == string::npos)
    {
        getline (ist,line);
        if (line.find("Tiempo") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            wei.timeSes = stoi (aux);
            cout << "Tiempo = " << wei.timeSes << endl;
        }
        if (line.find("Velocidad máxima") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            wei.velMax = stod (aux);
            cout << "Velocidad máxima: " << wei.velMax << endl;
        }
        if (line.find("Velocidad promedio") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            wei.velMed = atof (aux.c_str());
            cout << "Velocidad promedio: " << wei.velMed << endl;
        }
        if (line.find("Distancia") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            wei.distance = stof (aux);
            cout << "Distancia estimada: " << wei.distance << endl;
        }
        if (line.find("Calorias") != string::npos)
        {
            aux =line.substr(line.find_last_of(" ") +1);
            wei.calories = atof (aux.c_str());
            cout << "Calorias quemadas: " << wei.calories << endl;
        }
    }
    //Extracción de datos instantáneos: Llenado de los vectores con los datos instantáneos
    getline (ist,line);
    while (!ist.eof())
    {
        if (line.find("VELOCIDAD") != string::npos)
        {
            getline (ist,line);
            while (line.find("PPM") == string::npos)
            {
                wei.velocData.push_back(stod (line));
                getline (ist, line);
            }
        } else if (line.find("PPM") != string::npos){
            getline (ist,line);
            while (line.find("CARGA") == string::npos)
            {
                wei.pulseData.push_back(stod (line));
                getline (ist, line);
            }
        } else if (line.find("CARGA") != string::npos)
        {
            getline (ist,line);
            while (!ist.eof())
            {
                wei.dataOfLoad.push_back(stod (line));
                getline (ist, line);
            }
        }
    }

    // Líneas para mostrar los datos cargados, comentar si es necesario...
/*
    cout << "Datos de velocidad cargados: " << endl;
    for (int i = 0; i < (int) wei.velocData.size(); i++)
    {
        cout << wei.velocData[i] << endl;
    }
    cout << "Datos de pulso cargados: " << endl;
    for (int i = 0; i < (int) wei.pulseData.size(); i++)
    {
        cout << wei.pulseData[i] << endl;
    }
    cout << "Datos de carga cargados: " << endl;
    for (int i = 0; i < (int) wei.dataOfLoad.size(); i++)
    {
        cout << wei.dataOfLoad[i] << endl;
    }
*/
    return ist;
}
//--------------------------------------------------------------------------------------------------------
ostream& operator<< (ostream& ios, const WeightLoss& wei)
{
    ios << "-----ENTRENAMIENTO:WEIGHTLOSS-----" << endl;
    ios << "Usuario: " << wei.dataUser.name<< endl;
    ios << "DATOS DE ENTRENAMIENTO: " << endl;
    ios << "Fecha: " << wei.date << endl;
    ios << "Tiempo: " << wei.timeSes << endl;
    ios << "Velocidad máxima: " << wei.bike.vSensor->GetVelocMax() << endl;
    ios << "Velocidad promedio: " << wei.bike.vSensor->GetVelocProm() << endl;
    ios << "Distancia estimada: " << wei.distance << endl;
    ios << "Calorias: " << wei.calories << endl;
    ios << endl << endl << endl;
    ios << "Datos instantáneos:" << endl;
    ios << "VELOCIDAD" <<endl;
    for (int i = 0; i < (int) wei.velocData.size(); i++)
    {
        ios << wei.velocData [i] << endl;
    }
    ios << "PPM" <<endl;
    for (int i = 0; i < (int) wei.pulseData.size(); i++)
    {
        ios << wei.pulseData [i] << endl;
    }
    ios << "CARGA" <<endl;
    for (int i = 0; i < (int) wei.dataOfLoad.size(); i++)
    {
        ios << wei.dataOfLoad [i] << endl;
    }
    ios << "FIN_DATOS" << endl;
    return ios;
}

Free::Free(const string& name, const int& age, const char& sex, const float& weight, const float& height):Session (name, age, sex, weight, height)
{
    sessionType = "Free";
    //Se obtiene de forma automática la fecha la fecha en la que el usuario realiza la sesión
    //esto despues se utiliza despúes para guardar la sesión del usuario
    time_t now;
    time (&now);
    char *c = ctime (&now);
    string localDate (c);
    date = localDate;
    cout << "En constructor de free" << endl;
}

void Free::Start()
{
    //Método de inicio de entrenamiento
    try {
        sesAct = true;
        distance = 0.0;
        if (!bike.sensorsConfigured) bike.ConfigSerial(); //Configura el puerto serie
        screenMessage = "Entrenamiento iniciado";
    }  catch (int e) {
        if (e == ERROR_SERIAL_OPEN)
        {
            cout << "ERROR SERIAL" << endl;
            sesAct = false; //No inicia si no puede abrir el puerto serie
        }
    }
}

bool Free::Pause()
{
    //Evalúo los últimos 5 valores de velocidad. Si la suma de los mismos es cero retorna verdadero para pausar el entrenamiento automáticamente
    if (velocData.size()>5)
    {
        double auxData = 0.0;
        for (vector<double>::iterator it = velocData.end()-1; it!= velocData.end()-5; it--)
        {
            auxData+= *it;
        }
        if (auxData == 0.0)
        {
            return true;
        }
    }
    return false;
}

void Free::End()
{
    //Se desactiva las variables para evitar que siga realizando el muestreo de datos
    sesAct = false;
    paused = false;
}

void Free::Sample()
{
    //-----------------------------MÉTODO DE MUESTREO A EJECUTARSE REITERADAMENTE DURANTE LA SESIÓN ----------------------------
    if (sesAct == true)
    {
        //operaciones de muestreo de datos
        //una forma de usar esta función es que en la clase haya un objeto timer interno que se inicialice con
        //el método start - Encontré en internet el QTimer pero no se si usar ese

        timeSes++;
        //Actualizo datos de sensores
        bike.vSensor->GetValue();
        bike.pSensor->GetValue();
        bike.lSensor->GetValue();

        //Obtengo valores y guardo en vector
        velocData.push_back(bike.vSensor->GetVeloc());
        pulseData.push_back(bike.pSensor->GetPulse());
        dataOfLoad.push_back(bike.lSensor->GetLoad());
        distance+= bike.vSensor->GetVeloc(1)*sampleTime/3600; //obtengo distancia en km

        velMax = bike.vSensor->GetVelocMax();
        velMed = bike.vSensor->GetVelocProm();

        //obtengo los valores de calorias
        calories = CalcCalories();

        //Evaluo velocidad en un rango de variación del 10%
        if (Pause())
        {
            cout << "Entrenamiento pausado" << endl;
            sesAct = false;
            paused = true;
        }

        //evalúo PPM
        if(AlarmPpm())
        {
            screenMessage = "Frecuencia cardíaca alta";
        }

    }
    if (paused)
    {
        bike.vSensor->GetValue();
        screenMessage = "Entrenamiento pausado";
        if (bike.vSensor->GetVeloc()!= 0.0)
        {
            paused = false;
            sesAct = true;
        }
    }
}

void Free::WriteReport() const
{
    //Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
    fstream sessionFile;
    string fileName;
    fileName =  sessionType + string ("_") + dataUser.name + string ("_") + date;
    fileName.pop_back();
    sessionFile.open(fileName, ios::app);
    sessionFile << *this;
}

void Free::ReadReport()
{
    string fileName;
    fileName = date;
    fileName+= string ("_") += dataUser.name;
    fstream sessionFile;
    sessionFile.open(fileName, ios::in);
    if (!sessionFile) cout << "ERROR AL ABRIR EL ARCHIVO" << endl;
    sessionFile >> *this;
}


bool Free::AlarmPpm ()
{
    float freqMaxRef;
    if(dataUser.sex=='M')
    {
        freqMaxRef = 180 + ( 0.5 * dataUser.age );
    } else
    {
        freqMaxRef = 210.7 - ( 0.8 * dataUser.age );
    }
    if (pulseData.back() > freqMaxRef)
    {
        return true;
    }
    return false;
}

double Free::CalcCalories() const
{
    double cal = 0.0;
    double ind1 = 0.049;
    double indWeig = 2.2;
    double ind2 = 0.071;
    static int intLow=0, intHigh=0;
    if (velocData.back()<=70) intLow++;
    else intHigh++;
    cal = ind1 * ( dataUser.weight * indWeig ) * ( intLow * 0.016667 );
    cal += ind2 * ( dataUser.weight * indWeig ) * ( intHigh * 0.016667 );
    return cal;
}

ostream& operator<< (ostream& ios, const Free& free)
{
    ios << "-----ENTRENAMIENTO:FREE-----" << endl;
    ios << "Usuario: " << free.dataUser.name << endl;
    ios << "DATOS DE ENTRENAMIENTO: " << endl;
    ios << "Fecha: " << free.date << endl;
    ios << "Tiempo: " << free.timeSes << endl;
    ios << "Velocidad máxima: " << free.bike.vSensor->GetVelocMax() << endl;
    ios << "Velocidad promedio: " << free.bike.vSensor->GetVelocProm() << endl;
    ios << "Distancia estimada: " << free.distance << endl;
    ios << endl << endl << endl;
    ios << "Datos instantáneos:" << endl;
    ios << "VELOCIDAD" <<endl;
    for (int i = 0; i < (int) free.velocData.size(); i++)
    {
        ios << free.velocData [i] << endl;
    }
    ios << "PPM" <<endl;
    for (int i = 0; i < (int) free.pulseData.size(); i++)
    {
        ios << free.pulseData [i] << endl;
    }
    ios << "CARGA" <<endl;
    for (int i = 0; i < (int) free.dataOfLoad.size(); i++)
    {
        ios << free.dataOfLoad [i] << endl;
    }
    ios << "FIN_DATOS" << endl;
    return ios;
}

istream& operator>> (istream& ist, Free& free)
{
    //---------------OPERADOR >> PARA SESSION FREE -------------------------------------------
    // Permite armar el stream de entrada con los datos de entrenamiento de una sesión free y cargarlos en un objeto free

    string line, aux;
    while (line.find("Usuario") == string::npos )
    {
        getline (ist, line);
    }
    if (line.substr(line.find_first_of(" ")+1) != free.dataUser.name)
    {
        //Si el usuario del archivo no es coincidente con el usuario que inició sesión
        throw int (INVALID_USER);
        return ist;
    }
    std::setlocale(LC_NUMERIC,"C"); //Con esta linea permite reconocer al "." de los strings como delimitador de punto flotante
                                    //de lo contrario, tomaba "," entonces recortaba los números a su parte entera

    while (line.find("Datos instantáneos") == string::npos)
    {
        getline (ist,line);
        if (line.find("Tiempo") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            free.timeSes = stoi (aux);
            cout << "Tiempo = " << free.timeSes << endl;
        }
        if (line.find("Velocidad máxima") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            free.velMax = stod (aux);
            cout << "Velocidad máxima: " << free.velMax << endl;
        }
        if (line.find("Velocidad promedio") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            free.velMed = atof (aux.c_str());
            cout << "Velocidad promedio: " << free.velMed << endl;
        }
        if (line.find("Distancia") != string::npos)
        {
            aux = line.substr(line.find_last_of(" ") +1);
            free.distance = stof (aux);
            cout << "Distancia estimada: " << free.distance << endl;
        }
    }

    // Líneas para mostrar los datos cargados, comentar si es necesario...
/*
    cout << "Datos de velocidad cargados: " << endl;
    for (int i = 0; i < (int) free.velocData.size(); i++)
    {
        cout << free.velocData[i] << endl;
    }
    cout << "Datos de pulso cargados: " << endl;
    for (int i = 0; i < (int) free.pulseData.size(); i++)
    {
        cout << free.pulseData[i] << endl;
    }
    cout << "Datos de carga cargados: " << endl;
    for (int i = 0; i < (int) free.dataOfLoad.size(); i++)
    {
        cout << free.dataOfLoad[i] << endl;
    }
   */
    return ist;
}
