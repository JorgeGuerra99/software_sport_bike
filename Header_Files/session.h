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


enum {FILE_CONFIG_ERROR, INVALID_USER};
using namespace std;
class Session
{
public:
    /**
     * @brief GetLastData Retorna el último valor en alguno de los vectores de los datos sensados
     * @param a Indica de que sensor se extraerá el último dato, a='0' para sensor de pulso, a='1' para sensor de veocidad y a='2' para sensor de carga
     * @return Valor double con el último dato leído del sensor
     */
    double GetLastData (int a = 0) const;
    float GetCalories () const { return calories; }
    float GetDistance () const { return distance; }
    /**
     * @brief GetVelocMaxMed permite obtener el valor de la velociada máxima o media del entrenamiento
     * @param s su valor puede ser 0, 1 u cualquier otro valor
     * @return se retorna un valor del tipo double si s='0' devuelve la velocidad máxima, para s='1' retorna la velocidad media, en caso contrario retorna un -1
     */
    double GetVelocMaxMed (const int& s = 0);
    /**
     * @brief GetAllData permite extraer todos los datos obtenidos del sensor que se desee
     * @param sel puede ser los valores 'P', 'V' o 'L'
     * @return para sel='P' para el sensor de pulsos, sel='V' con el de velocidad y sel='L' para el sensor de carga
     */
    const vector <double> GetAllData (char sel = 'P') const;
    virtual void WriteReport () const = 0;
    /**
     * @brief timeSes Contador de MUESTRAS de la sesión
     */
    int timeSes = 0;
    /**
     * @brief screenMessage se guardan diferentes mensajes los cuales después son mostrados en las ventanas
     */
    string screenMessage = "Sesión de entrenamiento";
    /**
     * @brief sessionType se asigna el tipo de sesión que vaya a elegir el usuario
     */
    string sessionType;
    StateBike bike;
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
    Session () { cout << "Constructor por defecto: Session" << endl;};
    virtual ~Session () {cout << "Destructor de session" << endl;}
    virtual void Start () = 0;
    virtual void Sample () = 0; //muestreo: lectura de datos en conjunto con el timer
    virtual void LoadConfig () = 0;
    virtual bool Pause () = 0; //pausa de entrenamiento
    virtual void End () = 0;
    virtual void ReadReport () = 0;
    virtual bool AlarmPpm () = 0;
    virtual double CalcCalories ( )const = 0;
    //consultar: ¿Todos los métodos de una clase abstracta tienen que ser =0 ? ¿Pueden haber métodos en común?
    bool IsPaused () const { return paused; };
    /**
     * @brief velocData
     * @note correspondiente al contenedor vector
     * @detail es donde se van a guardar los valores provenientes del sensor de velocidad
     */
    vector < double > velocData;
    /**
     * @brief pulseData
     * @note correspondiente al contenedor vector
     * @detail es donde se van a guardar los valores provenientes del sensor de pulsos
     */
    vector < double > pulseData;
    /**
     * @brief dataOfLoad
     * @note correspondiente al contenedor vector
     * @detail es donde se van a guardar los valores provenientes del sensor de carga
     */
    vector < double > dataOfLoad;
    string date;
    /**
     * @brief sesAct
     * @note Flag que indica el estado de la sesión
     */
    bool sesAct = false;
    struct {
        string name;
        int age;
        char sex;
        float weight, height;
    } dataUser;
    /**
     * @brief paused
     * @note flag correspondiente al pausado de la sesión
     */
    bool paused = false;
    float calories = 0.0;
    float distance = 0.0;
    double velMax = 0.0;
    double velMed = 0.0;
};

//----------------------------------------------------------------------------------------------------------------------------------
//--------------------SESIÓN DE ENTRENAMIENTO: "CARDIO" ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Cardio class
 * @note Con la rutina ya realizada que consta de tramos de tiempo donde se debe ir aumentando la velocidad graudalmente
 */
class Cardio :public Session
{
public:
    /**
     * @brief Cardio - Constructor
     * @param name : nombre del usuario
     * @param age : edad del usaurio
     * @param sex : genero del usuario
     * @param weight : peso del usuario
     * @param height : altura del usuario
     */
    Cardio (const string& name, const int& age, const char& sex, const float& weight, const float& height); //por ahora necesitaría estos datos
    Cardio () { cout << "Constructor por defecto: Cardio" << endl; }
    virtual ~Cardio() { cout << "destructor cardio" << endl;}
    /**
     * @brief Start
     * @note inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause
     * @note deshabilita el muestreo de datos
     * @details evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause ();
    /**
     * @brief End
     * @note se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief WriteReport
     * @note Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
    virtual void ReadReport ();
    /**
     * @brief CalcCalories
     * @return devuelve un doble con las calorias quemadas durante la sesión
     */
    virtual double CalcCalories ( )const ;
    /**
     * @brief Sample
     * @note método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
    Cardio* operator* () { return this;}
private:
    /**
     * @brief NoRutAlm
     * @note alerta de desvio de rutina
     * @return retorna verdadero si no cumple con la rutina, en caso contrario retorna un falso
     * @details Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
     */
    bool NoRutAlm ();
    /**
     * @brief StageEval
     * @note Evalua etapa de entrenamiento de acuerdo al tiempo transcurrido
     */
    void StageEval (const int&);
    /**
     * @brief LoadConfig
     * @note asigna a los contenedores correspondientes la configuración de la sesión
     * @details extrae de un archivo txt la configuración del entrenamiento
     */
    virtual void LoadConfig ();
    /**
     * @brief AlarmPpm
     * @note alerta de pulsaciones
     * @details alerta al usuario de que tiene una frecuencia cardiaca que puede ser peligrosa
     * @param age : edad
     * @return retorna un verdadero en caso de superar la frecuencia máxima determinada
     */
    virtual bool AlarmPpm ( );
    friend ostream& operator<< (ostream& ios, const Cardio&);
    friend istream &operator>> ( istream& ist, Cardio &);
    int stage = 0;
    /**
     * @brief sampleTime
     * @note es el tiempo de muestreo
     */
    float sampleTime = 1;
    /**
     * @brief velocRef
     * @note perteneciente al contenedor de vector
     * @details es donde se asignan los valores pertenecientes a la configuración de sesión
     */
    vector < int > velocRef;
    /**
     * @brief timerRef
     * @note perteneciente al contenedor de vector
     * @details es donde se asignan los valores pertenecientes a la configuración de sesión
     */
    vector < int > timeRef;

};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "WEIGHTLOSS" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The WeightLoss class la idea es de cumplir con una cierta rutina ya configurada con una velocidad media constante durante un prologado tiempo
 */
class WeightLoss: public Session
{
public:
    /**
     * @brief WeightLoss - Constructor
     * @param name : nombre de usuario
     * @param age : edad del usuario
     * @param sex : genero del usuario
     * @param weight : peso del usuario
     * @param height : altura del usuario
     */
    WeightLoss (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    WeightLoss ();
    virtual ~WeightLoss() { cout << "destructor de weightloss" << endl;}
    /**
     * @brief Start
     * @note inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause
     * @note deshabilita el muestreo de datos
     * @details evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause () ;
    /**
     * @brief End
     * @note se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief WriteReport
     * @note Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
    virtual void ReadReport ();
    /**
     * @brief Sample
     * @note método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
    /**
     * @brief CalcCalories
     * @return devuelve un doble con las calorias quemadas durante la sesión
     */
    virtual double CalcCalories ( ) const ;
private:
    /**
     * @brief NoRutAlm
     * @note alerta de desvio de rutina
     * @return retorna verdadero si no cumple con la rutina, en caso contrario retorna un falso
     * @details Evalúa si la velocidad actual se mantiene dentro del rango del 10% de la velocidad de referencia
     */
    bool NoRutAlm();
    /**
     * @brief LoadConfig
     * @note asigna a los contenedores correspondientes la configuración de la sesión
     * @details extrae de un archivo txt la configuración del entrenamiento
     */
    virtual void LoadConfig ();
    /**
     * @brief AlarmPpm
     * @note alerta de pulsaciones
     * @details alerta al usuario de que tiene una frecuencia cardiaca que puede ser peligrosa
     * @param age: edad
     * @return retorna un verdadero en caso de superar la frecuencia máxima determinada
     */
    virtual bool AlarmPpm ();
    friend ostream& operator<< (ostream& ios, const WeightLoss& wei);
    friend istream& operator>> ( istream& ist, WeightLoss& wei);
    float timeRef;
    vector < float > intensityRef;
    float intensityMaxFc;
    float intensityMinFc;
    /**
     * @brief sampleTime
     * @note es el tiempo de muestreo
     */
    float sampleTime = 1;

};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "FREE" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Free class permite hacer un entrenamiento sin la necesidad de una rutina ya que solo obtiene los valores de los sensores a medida que el usuario entrena
 */
class Free: public Session
{
public:
    /**
     * @brief Free - Contructor
     * @param name : nombre de usuario
     * @param age : edad del usuario
     * @param sex : género del usuario
     * @param weight : peso del usuario
     * @param height : altura del usuario
     */
    Free (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    Free();
    virtual ~Free () { cout << "destructor de free"  << endl;}
    /**
     * @brief Start
     * @note inicia el entrenamiento - habilito sample y habilita el flag de sesAct
     */
    virtual void Start ();
    /**
     * @brief Pause
     * @note deshabilita el muestreo de datos
     * @details evalua los últimos 5 valores de velocidad, en caso de ser cero se deshabilita el muestreo de datos
     * @return retorna verdadero en caso de que la suma de cero
     */
    virtual bool Pause ();
    /**
     * @brief End
     * @note se desactiva las variables para evitar que siga realizando el muestreo de datos
     */
    virtual void End ();
    /**
     * @brief Sample
     * @note método de muestreo a ejecutarse reiteradamente durante la sesión
     */
    virtual void Sample ();
    /**
     * @brief WriteReport
     * @note Permite exportar unicamente la sesión realizada en ese instante en un archivo txt
     */
    virtual void WriteReport () const;
    virtual void ReadReport ();
    /**
     * @brief CalcCalories
     * @return devuelve un doble con las calorias quemadas durante la sesión
     */
    virtual double CalcCalories ( )const ;
private:
    /**
     * @brief AlarmPpm
     * @note alerta de pulsaciones
     * @details alerta al usuario de que tiene una frecuencia cardiaca que puede ser peligrosa
     * @param age : edad del usuario
     * @return retorna un verdadero en caso de superar la frecuencia máxima determinada
     */
    virtual bool AlarmPpm ( ) ;
    virtual void LoadConfig () {}
    friend ostream& operator<< (ostream& ios, const Free& free);
    friend istream& operator>> ( istream& ist, Free& free);
    /**
     * @brief sampleTime
     * @note es el tiempo de muestreo
     */
    float sampleTime = 1;
};


#endif // SESSION_H
