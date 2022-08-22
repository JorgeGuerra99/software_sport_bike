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
    vector < double > velocData;
    vector < double > pulseData;
    vector < double > dataOfLoad;
    StateBike bike;
    virtual void Start () = 0;
    virtual void Sample () = 0;
    virtual void LoadConfig () = 0;
    virtual void End () = 0;
    virtual void ViewReport () const = 0;
    virtual bool AlarmPpm ( const int &age) const = 0;
    virtual double CalcCalories ( const double &tim, const double &pes, const double &vel )const = 0;
    friend ostream& operator<< ( ostream& ios, const Session & );
    friend istream& operator>> ( istream& ist, const Session & );
};

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
    bool NoRutAlm () const;
    virtual void LoadConfig ();
    virtual bool AlarmPpm ( const int &age ) const;
};

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
    virtual void Sample ();
    virtual void LoadConfig ();
    virtual bool AlarmPpm (const int&) const;
};

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
