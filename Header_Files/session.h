/**
 * @file session.h
 * @version 1.0
 * @date 2023
 * @title Clase Session
 * @authors Bazán Maria, Guerra Jorge
 * @brief Este archivo contiene la declaración de la clase abstracta Session y cada una de sus clases derivadas correspondientes a los diferentes tipos de sesión, c/u con sus características
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
    bool alarm = false;
    StateBike bike;
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
protected:
    //-----Datos de sensores --------------
    vector < double > velocData;
    vector < double > pulseData;
    vector < double > dataOfLoad;
    Session (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    Session () { cout << "Constructor por defecto: Session" << endl;};
    virtual ~Session () {cout << "Destructor de session" << endl;}
    string date;
    bool sesAct = false;
    struct {
        string name;
        int age;
        char sex;
        float weight, height;
    } dataUser;
    bool paused = false;
    float calories = 0.0;
    float distance = 0.0;
    double velMax = 0.0;
    double velMed = 0.0;
    virtual void Start () = 0; //iniciar entrenamiento - habilito sample
    virtual void Sample () = 0; //muestreo: lectura de datos en conjunto con el timer
    virtual void LoadConfig () = 0;
    virtual bool Pause () = 0; //pausa de entrenamiento
    virtual void End () = 0;
    virtual void ReadReport () = 0;
    virtual bool AlarmPpm ( const int& age) = 0;
    virtual double CalcCalories ( )const = 0;
    //consultar: ¿Todos los métodos de una clase abstracta tienen que ser =0 ? ¿Pueden haber métodos en común?
    bool IsPaused () const { return paused; };
};

//----------------------------------------------------------------------------------------------------------------------------------
//--------------------SESIÓN DE ENTRENAMIENTO: "CARDIO" ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief The Cardio class con la rutina ya realizada que consta de tramos de tiempo donde se debe ir aumentando la velocidad graudalmente
 */
class Cardio :public Session
{
public:
    Cardio (const string& name, const int& age, const char& sex, const float& weight, const float& height); //por ahora necesitaría estos datos
    Cardio () { cout << "Constructor por defecto: Cardio" << endl; }
    virtual ~Cardio() { cout << "destructor cardio" << endl;}
    virtual void Start ();
    virtual bool Pause ();
    virtual void End ();
    virtual void WriteReport () const;
    virtual void ReadReport ();
    virtual double CalcCalories ( )const ;
    virtual void Sample ();
    Cardio* operator* () { return this;}
private:
    int stage = 0;
    float sampleTime = 1; //tiempo de muestreo
    vector < int > velocRef;
    vector < int > timeRef;
    bool NoRutAlm (); //Alerta de desvío de rutina
    void StageEval (const int&);
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int& age );
    friend ostream& operator<< (ostream& ios, const Cardio&);
    friend istream &operator>> ( istream& ist, Cardio &);
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
    WeightLoss (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    WeightLoss ();
    virtual ~WeightLoss() { cout << "destructor de weightloss" << endl;}
    virtual void Start ();
    virtual bool Pause () ;
    virtual void End ();
    virtual void WriteReport () const;
    virtual void ReadReport ();
    virtual void Sample ();
    virtual double CalcCalories ( ) const ;
private:
    float timeRef; //Está para el ejemplo 1 de 2 horas continuas, se puede agregar los otros ejemplos
    vector < float > intensityRef;
    float intensityMaxFc;
    float intensityMinFc;
    float sampleTime = 1;
    bool NoRutAlm();
    virtual void LoadConfig ();
    virtual bool AlarmPpm (const int& age);
    friend ostream& operator<< (ostream& ios, const WeightLoss& wei);
    friend istream& operator>> ( istream& ist, WeightLoss& wei);
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
    Free (const string& name, const int& age, const char& sex, const float& weight, const float& height);
    Free();
    virtual ~Free () { cout << "destructor de free"  << endl;}
    virtual void Start ();
    virtual bool Pause ();
    virtual void End ();
    virtual void Sample ();
    virtual void WriteReport () const;
    virtual void ReadReport ();
    virtual double CalcCalories ( )const ;
private:
    float sampleTime = 1;
    virtual bool AlarmPpm ( const int& age ) ;
    friend ostream& operator<< (ostream& ios, const Free& free);
    friend istream& operator>> ( istream& ist, Free& free);
};


#endif // SESSION_H
