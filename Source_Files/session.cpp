#include "Header_Files/session.h"

//-----------------------------------------------------------------------------------------
//------------------------------- MÉTODOS DE SESSION --------------------------------------
//-----------------------------------------------------------------------------------------
double Session ::CalcCalories (const double &time, const double &weig, const double &vel ) const
{
    double aux = 0;
    static double ind1 = 49 / 1000;
    static double indWeig = 22 / 10;
    static double ind2 = 71 / 1000;
    //corroborar que el número de la condición este en rpm
    if (vel <= 16.00)
    {
        aux = ind1 * ( weig * indWeig ) * ( time / 60 );
    }
    else
    {
        aux = ind2 * ( weig * indWeig ) * ( time / 60 );
    }
    return aux;
}


Session::Session(const User& myUser)
{
    dataUser = new User (myUser);
    cout << "En constructor de session" << endl;
}

//-----------------------------------------------------------------------------------------
//------------------------------- MÉTODOS DE CARDIO ---------------------------------------
//-----------------------------------------------------------------------------------------

Cardio::Cardio(const User& myUser): Session (myUser)
{
    SessionType = "Cardio";
    time_t now;
    time (&now);
    char *c = ctime (&now);
    string localDate (c);
    date = localDate;
    cout << "En constructor de cardio" << endl;
}

void Cardio::Start()
{
    try {
        sesAct = true;
        LoadConfig(); //cargo configuraciones del archivo
        distance = 0.0;
        if (!bike.sensorsConfigured) bike.ConfigSerial();
        screenMessage = "Entrenamiento iniciado";
    }  catch (int e) {
        if (e == ERROR_SERIAL_OPEN)
        {
            cout << "ERROR SERIAL" << endl;
            sesAct = false;
        }
    }
}

bool Cardio::Pause()
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

void Cardio::End()
{
    sesAct = false;
    paused = false;
}

void Cardio::ViewReport() const
{

}

void Cardio::WriteReport() const
{
    fstream sessionFile;
    string filename;
    filename = dataUser->nameUsr + string ("_") + date;
    filename.pop_back();
    sessionFile.open(filename, ios::app);
    sessionFile << *this;
}

void Cardio::ReadReport()
{
    fstream sessionFile;
    sessionFile.open("JorgeGuerra_Mon Jan 23 11:44:27 2023", ios::in);
    if (!sessionFile) cout << "ERROR AL ABRIR EL ARCHIVO" << endl;
    sessionFile >> *this;
}

double Cardio::CalcCalories(const double &tim, const double &pes, const double &vel) const
{

}

void Cardio::Sample()
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
        distance+= bike.vSensor->GetVeloc(1)*timeSes/sampleTime* 3.6; //obtengo distancia en km

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
        AlarmPpm(dataUser->age); //evalúo PPM
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

bool Cardio::NoRutAlm()
{
    //---------------------------------------------------------------------------------------------------------
    //Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
    //---------------------------------------------------------------------------------------------------------
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

void Cardio::StageEval(const int &tim)
{
    //Evalua etapa de entrenamiento de acuerdo al tiempo transcurrido
    for (int i=0; i < (int) timeRef.size(); i++)
    {
        if ((int) tim/sampleTime > timeRef[i])
        {
            stage = i+1;
        }
    }
}

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

    cout << "Datos cargados de tiempo: " << endl;
    for (int i = 0; i < (int)timeRef.size(); i++)
    {
        cout << timeRef [i] << endl;
    }
    cout << "Datos cargados de velocidad: " << endl;
    for (int i = 0; i < (int)velocRef.size(); i++)
    {
        cout << velocRef [i] << endl;
    }
}

bool Cardio::AlarmPpm(const int &age)
{
    float freqMaxRef = (220 - age)*0.85;
    if (pulseData.back() > freqMaxRef)
    {
        screenMessage = "Frecuencia cardíaca alta";
        return true;
    }
    return false;
}
ostream& operator<< (ostream& ios, const Cardio& car)
{
    ios << "-------------------------------SESIÓN DE ENTRENAMIENTO: CARDIO ----------------------------------------" << endl;
    ios << "Usuario: " << car.dataUser->nameUsr << endl;
    ios << "DATOS DE ENTRENAMIENTO: " << endl;
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
    return ios;
}

//-----------------------------------------------------------------------------------------
//------------------------------ MÉTODOS DE WEIGHTLOSS ------------------------------------
//-----------------------------------------------------------------------------------------

WeightLoss::WeightLoss (const User& myUser): Session (myUser)
{
    SessionType = "Weightloss";
    //Se obtiene de forma automática la fecha la fecha en la que el usuario realiza la sesión
    time_t now;
    time (&now);
    char *c = ctime (&now);
    string localDate (c);
    date = localDate;
    cout << "En constructor de weightloss" << endl;
}

void WeightLoss::Start ()
{
    try {
        sesAct = true;
        LoadConfig(); //cargo configuraciones del archivo
        distance = 0.0;
        if (!bike.sensorsConfigured) bike.ConfigSerial();
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

void WeightLoss::End ()
{
    sesAct = false;
    paused = false;
}

void WeightLoss::WriteReport () const
{
    fstream sessionFile;
    string filename;
    //definición del nombre de archivo en el que se guarda la sesión en formato "fecha-usuario.txt"
    filename = date;
    filename+= string ("_") += dataUser->nameUsr;
    // apertura del archivo de texto para posterior guardado de los datos de la sesión
    sessionFile.open(filename, ios::app);
//    sessionFile << *this;
}

void WeightLoss::ViewReport () const
{

}

void WeightLoss::IntensityFc (const int &age)
{
    //Se calcula los valores minimo y máximo de la intensidad al cual debe realizar esta sesión
    int Fcmax;
    Fcmax= 220 - age;
    intensityMinFc= Fcmax*(6/10);
    intensityMaxFc= Fcmax*(7/10);
}

istream& operator>> (istream& ist, Cardio& car)
{
    //---------------OPERADOR >> PARA SESSION CARDIO -------------------------------------------
    // Permite armar el stream de entrada con los datos de entrenamiento de una sesión cardio y cargarlos en un objeto cardio

    string line, aux;
    while (line.find("Usuario") == string::npos )
    {
        getline (ist, line);
    }
    if (line.substr(line.find_first_of(" ")+1) != car.dataUser->nameUsr)
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

    cout << "Datos de velocidad cargados: " << endl;
    for (int i = 0; i < (int) car.velocData.size(); i++)
    {
        cout << car.velocData[i] << endl;
    }
    cout << "Datos de pulso cargados: " << endl;
    for (int i = 0; i < (int) car.pulseData.size(); i++)
    {
        cout << car.pulseData[i] << endl;
    }
    cout << "Datos de carga cargados: " << endl;
    for (int i = 0; i < (int) car.dataOfLoad.size(); i++)
    {
        cout << car.dataOfLoad[i] << endl;
    }
    return ist;
}

istream& operator>> (istream& ist, WeightLoss& wei)
{

}
