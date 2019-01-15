#include <iostream>
#include <cmath>
#include <iomanip>
#include "GSL_func.hpp"

using namespace std;

#define PI 3.14159
#define N 50
double dx = 2*PI/(N-1.);

int main( int argc, char* argv[] ){
    double x[N];
    double y[N];

    double xmin=999,xmax=-999;

    for( int i=0; i<N; ++i ){
        x[i] = -PI + i*dx;
        y[i] = sin(x[i]);
        // cout.width(15);
        // cout << setw(15) << x[i] << " " << setw(15) << y[i] << endl;

        xmin = xmin > x[i] ? x[i] : xmin;
        xmax = xmax < x[i] ? x[i] : xmax;
    }


    GSL_Interpolator_1d interp1d(x,y,N,1);

    cout.precision(10);
    double t = -PI+0.023;
    while( t <= PI ){
        if( t>= xmin && t<=xmax ){
            double ft = interp1d.get_value(t);
            cout << "t = " << setw(15) << t << " " << setw(15) << ft << " value-diff: " << setw(15) << ft-sin(t) << endl;
        }
        t += dx;
    }

    return 0;
}