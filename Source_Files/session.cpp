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
