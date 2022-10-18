//-----------------------------CLASE SESSION ------------------------------------------------------------------
// Este archivo contiene la declaración de la clase abstracta Session y cada una de sus clases derivadas
// correspondientes a los diferentes tipos de sesión, c/u con sus características

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include <iostream>
#include "bike.h"

using namespace std;
class Session
{
protected:
    Session ( const int& age, const char& sex );
    virtual ~Session ();
    double time;
    const string date;
    bool sesAct = false;
    float freqMaxRef;
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
    friend ostream& operator<< ( ostream& ios, const Session & );
    friend istream& operator>> ( istream& ist, const Session & );
};

// Sesión de cardio
// Velocidad constante de a tramos. Se incrementa la velocidad gradualmente dando avisos y activando alertas
class Cardio :public Session
{
public:
    virtual void Start ();
    virtual void End ();
    virtual void ViewReport () const;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
private:
    float calories;
    float distance;
    int stage;
    vector < double > velocRef;
    vector < int > timeRef;
    virtual void Sample ();
    bool NoRutAlm () const; //Alerta de desvío de rutina
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int &age ) const;
};
// Clase pérdida de peso
// En esta rutina se trata de mantener un ritmo constante durante cierto período de tiempo
class WeightLoss: public Session
{
public:
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
