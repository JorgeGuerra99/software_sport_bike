#include "Header_Files/session.h"

double Session ::CalcCalories (const double &time, const double &pes, const double &vel ) const
{
    double aux = 0;
    static double ind1 = 49 / 1000;
    static double indPes = 22 / 10;
    static double ind2 = 71 / 1000;
    if (vel <= 16.00)
    {
        aux = ind1 * ( pes * indPes ) * ( time / 60 );
    }
    else
    {
        aux = ind2 * ( pes * indPes ) * ( time / 60 );
    }
    return aux;
}


Session::Session(const int &age, const char &sex)
{
    ageUser = age;
    sexUser = sex;
    cout << "En constructor de session" << endl;
}

Cardio::Cardio(const int &age, const char &sex): Session (age,sex)
{
    cout << "En constructor de cardio" << endl;
}

void Cardio::Start()
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
    cout << *this << endl;
}

void Cardio::ViewReport() const
{

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

        time++;
        //Actualizo datos de sensores
        bike.vSensor->GetValue();
        bike.pSensor->GetValue();
        bike.lSensor->GetValue();

        //Obtengo valores y guardo en vector
        velocData.push_back(bike.vSensor->GetVeloc());
        pulseData.push_back(bike.pSensor->GetPulse());
        dataOfLoad.push_back(bike.lSensor->GetLoad());
        distance+= bike.vSensor->GetVeloc(1)*time/sampleTime* 3.6; //obtengo distancia en km

        //Evalúo etapa actual
        StageEval(time);
        //Evaluo velocidad en un rango de variación del 10%
        if (NoRutAlm()){
            //acá lo que voy a hacer si no cumple las especificaciones de velocidad
            cout << "no cumple esp." << endl;
        }
        if (Pause())
        {
            cout << "Entrenamiento pausado" << endl;
            sesAct = false;
            paused = true;
        }
        AlarmPpm(ageUser); //evalúo PPM
    }
    if (paused)
    {
        bike.vSensor->GetValue();
        if (bike.vSensor->GetVeloc()!= 0.0)
        {
            paused = false;
            sesAct = true;
        }
    }
}

bool Cardio::NoRutAlm() const
{
    //---------------------------------------------------------------------------------------------------------
    //Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
    //---------------------------------------------------------------------------------------------------------
    if (velocData.back() > velocRef[stage]+ velocRef[stage]*0.1)
    {
        cout << "Reduzca la velocidad" << endl;
        return true;
    }
    if (velocData.back() < velocRef[stage] - velocRef[stage]*0.1)
    {
        cout << "Incremente la velocidad" << endl;
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
        cout << "Error al abrir Session1Config" << endl;
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

bool Cardio::AlarmPpm(const int &age) const
{
    float freqMaxRef = (220 - age)*0.85;
    if (pulseData.back() > freqMaxRef)
    {
        cout << "Frecuencia cardíaca alta" << endl;
        return true;
    }
    return false;
}
ostream& operator<< ( ostream& ios, const Session & ){
    ios << "Datos de velocidad" << endl;
    ios << "Datos de tiempo" << endl;
    return ios;
}
