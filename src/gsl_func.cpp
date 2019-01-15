#include "inspector.hpp"

GSL_Interpolator_1d::GSL_Interpolator_1d(){
    x_min = x_max = 0;
    x = NULL;
    fx= NULL;
    size = 0;
    interp1d = NULL;
    accel = NULL;
}

GSL_Interpolator_1d::GSL_Interpolator_1d( double x_[], double fx_[], int size_, int kind ){
    init(x_,fx_,size_,kind);
}    

GSL_Interpolator_1d::~GSL_Interpolator_1d(){
    if( x != NULL ) delete[] x;
    if( fx != NULL ) delete[] fx;
    if( accel != NULL ) gsl_interp_accel_free(accel);
    if( interp1d != NULL ) gsl_spline_free(interp1d);
}

void GSL_Interpolator_1d::init( double x_[], double fx_[], int size_, int kind ){
    x_min = x_max = 0;
    x = new double[size_];
    fx= new double[size_];
    size = size_;

    for( int i=0; i<size; ++i ){
        x[i] = x_[i];
        fx[i] = fx_[i];

        if( x[i] < x_min )
            x_min = x[i];
        if( x[i] > x_max )
            x_max = x[i];
    }

    accel = gsl_interp_accel_alloc();
    switch (kind){
    case 0:
        interp1d = gsl_spline_alloc(gsl_interp_linear,size_);
        break;
    case 1:
        interp1d = gsl_spline_alloc(gsl_interp_cspline,size_);
        break;
    default:
        std::cout << "FILE: " << __FILE__ 
                  << "LINE: " << __LINE__
                  << "Error info: Unkonw kind " << kind << std::endl;
        break;
    }
    gsl_spline_init(interp1d,x,fx,size);
}

double GSL_Interpolator_1d::get_value( double xi ){
    if( xi < x_min ){
        std::string errmsg;
        std::ostringstream stream_xi, stream_x_min;
        stream_xi << xi;
        stream_x_min << x_min;
        errmsg += "xi = " + stream_xi.str() + " is less than x_min = " + stream_x_min.str();
        PrintError(errmsg);
    }
    else if( xi > x_max ){
        std::string errmsg;
        std::ostringstream stream_xi, stream_x_max;
        stream_xi << xi;
        stream_x_max << x_max;
        errmsg += "xi = " + stream_xi.str() + " is greater than x_max = " + stream_x_max.str();
        PrintError(errmsg);
    }
    return gsl_spline_eval(interp1d,xi,accel);
}

//  Get inverse of the given 3x3 matrix
int GetMatrixInverse( gsl_matrix* m, gsl_matrix* inv_m ){
    int i,j,signum;
    gsl_matrix *LU = gsl_matrix_alloc(3,3);
    gsl_permutation *p = gsl_permutation_alloc(3);

    for( i=0; i<3; i++ ){
        for( j=0; j<3; j++){
            gsl_matrix_set(LU,i,j,gsl_matrix_get(m,i,j));
        }
    }

    gsl_linalg_LU_decomp(LU,p,&signum);
    gsl_linalg_LU_invert(LU,p,inv_m);

    gsl_matrix_free(LU);
    gsl_permutation_free(p);

    return 0;
}