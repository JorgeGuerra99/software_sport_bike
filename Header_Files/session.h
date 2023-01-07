//-----------------------------CLASE SESSION ------------------------------------------------------------------
// Este archivo contiene la declaración de la clase abstracta Session y cada una de sus clases derivadas
// correspondientes a los diferentes tipos de sesión, c/u con sus características

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "bike.h"

enum {FILE_CONFIG_ERROR};
using namespace std;
class Session
{
protected:
    Session ( const int& age, const char& sex );
    virtual ~Session () {cout << "Destructor de session" << endl;};
    int time = 0;
    const string date;
    bool sesAct = false;
    float freqMaxRef;
    int ageUser;
    char sexUser;
    //-----Datos de sensores --------------
    vector < double > velocData;
    vector < double > pulseData;
    vector < double > dataOfLoad;
    // ---------- Objeto bike ---------------
    StateBike bike;
    //------- Métodos de sesión ------------
    virtual void Start () = 0; //iniciar entrenamiento - habilito sample
    virtual void Sample () = 0; //muestreo: lectura de datos en conjunto con el timer
    virtual void LoadConfig () = 0;
    virtual void End () = 0;
    virtual void ViewReport () const = 0;
    virtual bool AlarmPpm ( const int &age) const = 0;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel )const = 0;
    // -------- Operadores de I/O ----------------
    friend ostream& operator<< ( ostream& ios, const Session & ); //Se utiliza friend porque los operadores >> y << no son de la clase Session
                                                                  //pero pueden acceder a los miembros de esta
    friend istream& operator>> ( istream& ist, const Session & );
};

//----------------------------------------------------------------------------------------------------------------------------------
//--------------------SESIÓN DE ENTRENAMIENTO: "CARDIO" ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Velocidad constante de a tramos. Se incrementa la velocidad gradualmente dando avisos y activando alertas
class Cardio :public Session
{
public:
    Cardio (const int& age, const char& sex); //por ahora necesitaría estos datos
    ~Cardio() { cout << "destructor cardio" << endl;}
    virtual void Start ();
    virtual void End ();
    virtual void ViewReport () const;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
    virtual void Sample ();
private:
    float calories;
    float distance;
    int stage = 0;
    float sampleTime = 1; //tiempo de muestreo
    vector < int > velocRef;
    vector < int > timeRef;
    bool NoRutAlm () const; //Alerta de desvío de rutina
    void StageEval (const int&);
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int &age ) const;
};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "WEIGHTLOSS" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
// En esta rutina se trata de mantener un ritmo constante durante cierto período de tiempo
class WeightLoss: public Session
{
public:
    WeightLoss (const int& age, const char& sex);
    virtual void Start ();
    virtual void End ();
    bool VelCte () const;
    virtual void ViewReport () const;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
private:
    float calories;
    float distance;
    float velocRef;
    virtual void Sample ();
    virtual void LoadConfig ();
    virtual bool AlarmPpm (const int&) const;
};

// Clase libre
// Libre uso sin restricciones
class Free: public Session
{
public:
    Free (const int& age, const char & sex);
    virtual void Start ();
    virtual void End ();
    virtual void ViewReport () const ;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
private:
    float calories;
    float distance;
    virtual void Sample ();
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int& ) const ;
};




#endif // SESSION_H
