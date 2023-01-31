//-----------------------------CLASE SESSION ------------------------------------------------------------------
// Este archivo contiene la declaración de la clase abstracta Session y cada una de sus clases derivadas
// correspondientes a los diferentes tipos de sesión, c/u con sus características

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "bike.h"
#include "Header_Files/user.h"


enum {FILE_CONFIG_ERROR, INVALID_USER};
using namespace std;
class Session
{
public:
    int timeSes = 0;
    //-----Datos de sensores --------------
    vector < double > velocData;
    vector < double > pulseData;
    vector < double > dataOfLoad;
    string screenMessage = "Sesión de entrenamiento";
protected:
    Session (const User&);
    virtual ~Session () {cout << "Destructor de session" << endl;};
    string date;
    bool sesAct = false;
    User* dataUser;
    string SessionType;
    bool paused = false;
    // ---------- Objeto bike ---------------
    StateBike bike;
    //------- Métodos de sesión ------------
    virtual void Start () = 0; //iniciar entrenamiento - habilito sample
    virtual void Sample () = 0; //muestreo: lectura de datos en conjunto con el timer
    virtual void LoadConfig () = 0;
    virtual bool Pause () = 0; //pausa de entrenamiento
    virtual void End () = 0;
    virtual void ViewReport () const = 0;
    virtual void WriteReport () const = 0;
    virtual void ReadReport () = 0;
    virtual bool AlarmPpm ( const int &age) = 0;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel )const = 0;
    bool IsPaused () const { return paused; };
};

//----------------------------------------------------------------------------------------------------------------------------------
//--------------------SESIÓN DE ENTRENAMIENTO: "CARDIO" ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Velocidad constante de a tramos. Se incrementa la velocidad gradualmente dando avisos y activando alertas
class Cardio :public Session
{
public:
    Cardio (const User&); //por ahora necesitaría estos datos
    ~Cardio() { cout << "destructor cardio" << endl;}
    virtual void Start ();
    virtual bool Pause ();
    virtual void End ();
    virtual void ViewReport () const;
    virtual void WriteReport () const;
    virtual void ReadReport ();
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
    virtual void Sample ();
private:
    float calories;
    float distance;
    int stage = 0;
    float sampleTime = 1; //tiempo de muestreo
    double velMax;
    double velMed;
    vector < int > velocRef;
    vector < int > timeRef;
    bool NoRutAlm (); //Alerta de desvío de rutina
    void StageEval (const int&);
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int &age );
    friend ostream& operator<< (ostream& ios, const Cardio&);
    friend istream &operator>> ( istream& ist, Cardio &);
};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "WEIGHTLOSS" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
// En esta rutina se trata de mantener un ritmo constante durante cierto período de tiempo
class WeightLoss: public Session
{
public:
    WeightLoss (const User&);
    ~WeightLoss() { cout << "destructor de weightloss"<<endl;}
    virtual void Start ();
    virtual bool Pause () ;
    virtual void End ();
    //bool VelCte () const;
    virtual void ViewReport () const;
    virtual void WriteReport () const;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const {};
private:
    float calories;
    float distance;
    //float velocRef; lo saco porque lo hago de acuerdo a un rango de pulsaciones de la persona
    float timeRef; //Está para el ejemplo 1 de 2 horas continuas, se puede agregar los otros ejemplos
    float intensityMaxFc;
    float intensityMinFc;
    void IntensityFc (const int &age);  //asigna los valores de intensidad de FC max y min
    bool NoRutAlm() const;
    virtual void Sample () {};
    virtual void LoadConfig () {};
    virtual bool AlarmPpm (const int &age) const {};
    friend ostream& operator<< (ostream& ios, const WeightLoss&);
};

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------SESIÓN DE ENTRENAMIENTO: "FREE" ----------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
// Libre uso sin restricciones
class Free: public Session
{
public:
    Free (const User&);
    virtual void Start ();
    virtual void End ();
    virtual void ViewReport () const ;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel ) const;
private:
    float calories;
    float distance;
    virtual void Sample ();
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int& ) ;
};


#endif // SESSION_H
