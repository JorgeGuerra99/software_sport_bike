/**
 * @file session.h
 * @date 2023
 * @brief Este archivo contiene la declaración de la clase abstracta Session y cada una de sus clases derivadas correspondientes a los diferentes tipos de sesión, c/u con sus características
 * @authors Bazán Maria, Guerra Jorge
*/

#ifndef SESSION_H
#define SESSION_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "bike.h"

/**
 *@brief enum: Utilizado para contener errores de diferentes tipos
 *@details Error en el archivo de configuración, usuario invalido
 */
enum {FILE_CONFIG_ERROR, INVALID_USER};
using namespace std;
/**
 * @brief The Session class: Clase abstracta
 */
class Session
{
public:
    /**
     * @brief GetLastData: Retorna el último valor en alguno de los vectores de los datos sensados
     * @param a: Indica de que sensor se extraerá el último dato, a='0' para sensor de pulso, a='1' para sensor de veocidad y a='2' para sensor de carga
     * @return Retorna el valor double con el último dato leído del sensor
     */
    double GetLastData (const int& a = 0) const;
    /**
     * @brief GetCalories: Permite obtener el valor de las calorias quemadas en la sesión
     * @return Retorna el valor float de las calorias
     */
    float GetCalories () const { return calories; }
    /**
     * @brief GetDistance: Permite obterner el valor de la distancia realizada en la sesión
     * @return Retorna el valor floar de la distancia en kilometros
     */
    float GetDistance () const { return distance; }
    /**
     * @brief GetVelocMaxMed: Permite obtener el valor de la velociada máxima o media del entrenamiento
     * @param s: Su valor puede ser 0, 1 u cualquier otro valor
     * @return Retorna un valor del tipo double si s='0' devuelve la velocidad máxima, para s='1' retorna la velocidad media, en caso contrario retorna un -1
     */
    double GetVelocMaxMed (const int& s = 0) const;
    /**
     * @brief GetAllData: Permite extraer todos los datos obtenidos del sensor que se desee
     * @param sel: Puede ser los valores 'P', 'V' o 'L'
     * @return Con sel='P' para el sensor de pulsos, sel='V' con el de velocidad y sel='L' para el sensor de carga
     */
    const vector <double> GetAllData (const char& sel = 'P') const;
    /**
     * @brief WriteReport: Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const = 0;
    /**
     * @brief nameSession: Propiedad donde se almacena el nombre de la sesión realizada
     */
    string nameSession;
    /**
     * @brief timeSes: Contador de MUESTRAS de la sesión
     */
    int timeSes = 0;
    /**
     * @brief screenMessage: Se guardan diferentes mensajes los cuales después son mostrados en las ventanas
     */
    string screenMessage = "Sesión de entrenamiento";
    /**
     * @brief sessionType: Se asigna el tipo de sesión que vaya a elegir el usuario
     */
    string sessionType;
    /**
     * @brief bike: Objeto de la clase StateBike
     */
    StateBike bike;
    friend ostream& operator<< (ostream& os, const Session& session);
protected:
    /**
     * @brief Session - Constructor
     * @param name : nombre del usuario
     * @param age : edad del usuario
     * @param sex : genero del usuario
     * @param weight : peso del usurio
     * @param height : altura del usuario
     */
    Session (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    /**
     * @brief Session - Constructor
     */
    Session () { cout << "Constructor por defecto: Session" << endl;};
    virtual ~Session () {cout << "Destructor de session" << endl;}
    /**
     * @brief Start: Inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start () = 0;
    /**
     * @brief Sample: Método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample () = 0;
    /**
     * @brief LoadConfig: Asigna a los contenedores correspondientes la configuración de la sesión
     * @details Extrae de un archivo txt la configuración del entrenamiento
     */
    virtual void LoadConfig () = 0;
    /**
     * @brief Pause: Deshabilita el muestreo de datos
     * @details Evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return Retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause () = 0;
    /**
     * @brief End: Se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End () = 0;
    /**
     * @brief AlarmPpm: Alerta de pulsaciones
     * @details Alerta al usuario de que tiene una frecuencia cardiaca que puede ser peligrosa
     * @return Retorna un verdadero en caso de superar la frecuencia máxima determinada
     */
    virtual bool AlarmPpm ();
    /**
     * @brief CalcCalories: Realiza el calculo de calorias quemadas mientras transcurre la sesión
     * @return Retorna un doble con las calorias quemadas durante la sesión
     */
    virtual double CalcCalories ( )const;
    /**
     * @brief Print: Permite llamar a la sobrecarga del operador correspondiente
     */
    virtual void Print (ostream& os) const = 0;
    /**
     * @brief IsPaused: Permite saber si la sesión se encuentra pausada
     * @return Retorna un valor que de ser verdadero la sesión fue pausada
     */
    bool IsPaused () const { return paused; };
    /**
     * @brief velocData: Correspondiente al contenedor vector donde se van a guardar los valores provenientes del sensor de velocidad
     */
    vector < double > velocData;
    /**
     * @brief pulseData: Correspondiente al contenedor vector donde se van a guardar los valores provenientes del sensor de pulsos
     */
    vector < double > pulseData;
    /**
     * @brief dataOfLoad: Correspondiente al contenedor vector donde se van a guardar los valores provenientes del sensor de carga
     */
    vector < double > dataOfLoad;
    /**
     * @brief date: Propiedad donde se almacena la fecha de la sesión realizada
     */
    string date;
    /**
     * @brief sesAct: Flag que indica el estado de la sesión
     */
    bool sesAct = false;
    /**
     * @brief dataUser: Estructura donde se almacena los datos del usuario
     */
    struct {
        string name;
        int age;
        char sex;
        float weight, height;
    } dataUser;
    /**
     * @brief paused: Flag correspondiente a la pausa de la sesión
     */
    bool paused = false;
    /**
     * @brief calories: Propiedad donde se almacenan las calorias quemadas durante la sesión
     */
    float calories = 0.0;
    /**
     * @brief distance: Propiedad donde se almacena la distancia recorrida durante la sesión
     */
    float distance = 0.0;
    /**
     * @brief velMax: Propiedad donde se almacena la velocidad máxima de la sesión
     */
    double velMax = 0.0;
    /**
     * @brief velMed: Propiedad donde se almacena la velocidad media o promedio la sesión realizada
     */
    double velMed = 0.0;
    /**
     * @brief sampleTime: Es el tiempo de muestreo
     */
    const float sampleTime = 1;
    /**
     * @brief ppmMax: Valor donde se asigna el máximo valor de pulsación para el usuario
     */
    float ppmMax;
};

//----------------------------------------------------------------------------------------------------------------------------------
//--------------------SESIÓN DE ENTRENAMIENTO: "CARDIO" ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Cardio class: Con la rutina ya realizada que consta de tramos de tiempo donde se debe ir aumentando la velocidad graudalmente
 */
class Cardio :public Session
{
public:
    /**
     * @brief Cardio - Constructor
     * @param name : Nombre del usuario
     * @param age : Edad del usuario
     * @param sex : Genero del usuario
     * @param weight : Peso del usuario
     * @param height : Altura del usuario
     */
    Cardio (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    /**
     * @brief Cardio - Constructor
     */
    Cardio () { cout << "Constructor por defecto: Cardio" << endl; }
    virtual ~Cardio() { cout << "destructor cardio" << endl;}
    /**
     * @brief Start: Inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause: Deshabilita el muestreo de datos
     * @details Evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return Retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause ();
    /**
     * @brief End: Se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief WriteReport: Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
    /**
     * @brief Sample: Método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
    Cardio* operator* () { return this;}
private:
    /**
     * @brief NoRutAlm: Alerta de desvio de rutina
     * @return Retorna verdadero si no cumple con la rutina, en caso contrario retorna un falso
     * @details Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
     */
    bool NoRutAlm ();
    /**
     * @brief StageEval: Evalua etapa de entrenamiento de acuerdo al tiempo transcurrido
     */
    void StageEval (const int& tim);
    /**
     * @brief LoadConfig: Asigna a los contenedores correspondientes la configuración de la sesión
     * @details Extrae de un archivo txt la configuración del entrenamiento
     */
    virtual void LoadConfig ();
    /**
     * @brief Print: Permite le ejecución de la sobrecarga del operador para esta clase
     */
    void Print (ostream& os) const;
    friend ostream& operator<< (ostream& ios, const Cardio& car);
    friend istream &operator>> ( istream& ist, Cardio& car);
    /**
     * @brief stage: Propiedad que indica la etapa de la sesión en la que se encuentra
     */
    int stage = 0;
    /**
     * @brief velocRef: Perteneciente al contenedor de vector donde se asignan los valores pertenecientes a la configuración de sesión
     */
    vector < int > velocRef;
    /**
     * @brief timerRef: Perteneciente al contenedor de vector donde se asignan los valores pertenecientes a la configuración de sesión
     */
    vector < int > timeRef;

};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "WEIGHTLOSS" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The WeightLoss class: La idea es de cumplir con una cierta rutina ya configurada con una velocidad media constante durante un prologado tiempo
 */
class WeightLoss: public Session
{
public:
    /**
     * @brief WeightLoss - Constructor
     * @param name: Nombre de usuario
     * @param age: Edad del usuario
     * @param sex: Genero del usuario
     * @param weight: Peso del usuario
     * @param height: Altura del usuario
     */
    WeightLoss (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    /**
     * @brief WeightLoss - Constructor
     */
    WeightLoss (){ cout << "Constructor por defecto: WeightLoss" << endl; }
    virtual ~WeightLoss() { cout << "destructor de weightloss" << endl;}
    /**
     * @brief Start: Inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause: Deshabilita el muestreo de datos
     * @details Evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return Retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause () ;
    /**
     * @brief End: Se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief WriteReport: Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
    /**
     * @brief Sample: Método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
private:
    /**
     * @brief NoRutAlm: Alerta de desvio de rutina
     * @return Retorna verdadero si no cumple con la rutina, en caso contrario retorna un falso
     * @details Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
     */
    bool NoRutAlm();
    /**
     * @brief LoadConfig: Asigna a los contenedores correspondientes la configuración de la sesión
     * @details Extrae de un archivo txt la configuración del entrenamiento
     */
    virtual void LoadConfig ();
    /**
     * @brief Print: Permite le ejecución de la sobrecarga del operador para esta clase
    */
    void Print (ostream& os) const;
    friend ostream& operator<< (ostream& ios, const WeightLoss& wei);
    friend istream& operator>> ( istream& ist, WeightLoss& wei);
    /**
     * @brief timeRef: Permite el almacenamiento del tiempo que debe cumplir la sesión
     * @details El valor que se debe almacenar tiene que encontrarse en segundos
     */
    float timeRef;
    /**
     * @brief intensityMaxFc: Propiedad donde se almacena el valor de intensidad máxima que debe realizar el usario
     * @details Este se determina de acuerdo a los datos de archivo de configuraciones y factores como edad y genero del usuario
     */
    float intensityMaxFc;
    /**
     * @brief intensityMinFc: Propiedad donde se almacena el valor de intensidad mínimo que debe realizar el usario
     * @details Este se determina de acuerdo a los datos de archivo de configuraciones y factores como edad y genero del usuario
     */
    float intensityMinFc;
};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "FREE" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Free class: Permite hacer un entrenamiento sin la necesidad de una rutina ya que solo obtiene los valores de los sensores a medida que el usuario entrena
 */
class Free: public Session
{
public:
    /**
     * @brief Free - Contructor
     * @param name : Nombre de usuario
     * @param age : Edad del usuario
     * @param sex : Género del usuario
     * @param weight : Peso del usuario
     * @param height : Altura del usuario
     */
    Free (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    /**
     * @brief Free - Constructor
     */
    Free(){ cout << "Constructor por defecto: Free" << endl; }
    virtual ~Free () { cout << "destructor de free"  << endl;}
    /**
     * @brief Start: Inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause: Deshabilita el muestreo de datos
     * @details Evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return Retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause ();
    /**
     * @brief End: Se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief Sample: Método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
    /**
     * @brief WriteReport: Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
private:
    virtual void LoadConfig () {}
    /**
     * @brief Print: Permite le ejecución de la sobrecarga del operador para esta clase
     */
    void Print (ostream& os) const ;
    friend ostream& operator<< (ostream& ios, const Free& free);
    friend istream& operator>> ( istream& ist, Free& free);
};


#endif // SESSION_H
