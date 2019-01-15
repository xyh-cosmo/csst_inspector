#include <iostream>
#include <sstream>

#include <gsl/gsl_interp.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "misc.hpp"

#ifndef _GSL_FUNC_HPP_
#define _GSL_FUNC_HPP_

class GSL_Interpolator_1d{
private:
    double *x,x_min,x_max;
    double *fx;
    int size;
    gsl_spline *interp1d;
    gsl_interp_accel *accel;
public:

    GSL_Interpolator_1d();
    GSL_Interpolator_1d( double x_[], double fx_[], int size_, int kind=0 );
    ~GSL_Interpolator_1d();
    void init( double x_[], double fx_[], int size_, int kind );
    double get_value( double xi );
};


int GetMatrixInverse( gsl_matrix* m, gsl_matrix* inv_m );

#endif