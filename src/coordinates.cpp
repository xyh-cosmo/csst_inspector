#include "inspector.hpp"

//  convert Cartesian coordinates into 2D spherical coordinates
void Cartesian2Spherical_ra_dec_radian(double* X, double& ra, double& dec){

    double r = sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    
    dec = asin(X[2]/r);

    if( fabs(X[0]/r) > 1e-8 && fabs(X[1]/r) > 1e-8 ){
        ra = 2.0*atan(X[1]/(r*cos(dec)+X[0]));
    }
    else{
        double x = X[0]*1e8;
        double y = X[1]*1e8;
        ra = 2.0*atan(y/(sqrt(x*x+y*y)+x));
    }
    
    if( ra < 0.0 )
        ra += 2.0*M_PI;
}

void Cartesian2Spherical_ra_dec_degree(double* X, double& ra, double& dec){
    
    double r = sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    
    dec = asin(X[2]/r);

    if( fabs(X[0]/r) > 1e-8 && fabs(X[1]/r) > 1e-8 ){
        ra = 2.0*atan(X[1]/(r*cos(dec)+X[0]));
    }
    else{
        double x = X[0]*1e8;
        double y = X[1]*1e8;
        ra = 2.0*atan( y/(sqrt(x*x+y*y)+x) );
    }

    ra  *= 180.0/M_PI;
    dec *= 180.0/M_PI;

    if( ra < 0 )
        ra += 360;
}

void Cartesian2Spherical_theta_phi_radian(double* X, double& theta, double& phi){
    double r = sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    double ra,dec;

    dec = asin(X[2]/r);
    
    if( fabs(X[0]/r) > 1e-8 && fabs(X[1]/r) > 1e-8 ){
        ra = 2.0*atan(X[1]/(r*cos(dec)+X[0]));
    }
    else{
        double x = X[0]*1e8;
        double y = X[1]*1e8;
        ra = 2.0*atan(y/(sqrt(x*x+y*y)+x));
    }
    
    phi   = ra;
    theta = 0.5*M_PI-dec;

    if( phi < 0 )
        phi += 2*M_PI;
}

void Cartesian2Spherical_theta_phi_degree(double* X, double& theta, double& phi){
    double r = sqrt(X[0]*X[0] + X[1]*X[1] + X[2]*X[2]);
    double ra,dec;

    dec = asin(X[2]/r);

    if( fabs(X[0]/r) > 1e-8 && fabs(X[1]/r) > 1e-8 ){
        ra = 2.0*atan(X[1]/(r*cos(dec)+X[0]));
    }
    else{
        double x = X[0]*1e8;
        double y = X[1]*1e8;
        ra = 2.0*atan(y/(sqrt(x*x+y*y)+x));
    }
    
    phi   = ra;
    theta = 0.5*M_PI-dec;

    theta *= 180.0/M_PI;
    phi   *= 180.0/M_PI;

    if( phi < 0 )
        phi += 360;
}

void Spherical2Cartesian_ra_dec_radian(double ra, double dec, double x[]){
    x[0] = cos(dec)*cos(ra);
    x[1] = cos(dec)*sin(ra);
    x[2] = sin(dec);
}

void Spherical2Cartesian_ra_dec_degree(double ra, double dec, double x[]){
    // std::cout << "calling " << __FUNCTION__ << "\n";
    x[0] = cos(dec*M_PI/180)*cos(ra*M_PI/180);
    x[1] = cos(dec*M_PI/180)*sin(ra*M_PI/180);
    x[2] = sin(dec*M_PI/180);
}

void Spherical2Cartesian_theta_phi_radian(double theta, double phi, double x[]){
    x[0] = sin(theta)*cos(phi);
    x[1] = sin(theta)*sin(phi);
    x[2] = cos(theta);
}

void Spherical2Cartesian_theta_phi_degree(double theta, double phi, double x[]){
    x[0] = sin(theta*M_PI/180)*cos(phi*M_PI/180);
    x[1] = sin(theta*M_PI/180)*sin(phi*M_PI/180);
    x[2] = cos(theta*M_PI/180);
}


//  transform equatorial coordinates to ecliptical coordinates
Equatorial2Ecliptic::Equatorial2Ecliptic(){
    xeq = gsl_vector_alloc(3);
    xec = gsl_vector_alloc(3);
    R = gsl_matrix_alloc(3,3);

    gsl_matrix_set(R,0,0,1);
    gsl_matrix_set(R,0,1,0);
    gsl_matrix_set(R,0,2,0);
    gsl_matrix_set(R,1,0,0);
    gsl_matrix_set(R,1,1,0.917392);
    gsl_matrix_set(R,1,2,0.397983);
    gsl_matrix_set(R,2,0,0);
    gsl_matrix_set(R,2,1,-0.397983);
    gsl_matrix_set(R,2,2,0.917392);
}

Equatorial2Ecliptic::~Equatorial2Ecliptic(){
    if( R != NULL )
        gsl_matrix_free(R);
    if( xeq != NULL )
        gsl_vector_free(xeq);
    if( xec != NULL )
        gsl_vector_free(xec);
}

void Equatorial2Ecliptic::transform(double *x_eq, double *x_ec ){
    gsl_vector_set(xeq,0,x_eq[0]);
    gsl_vector_set(xeq,1,x_eq[1]);
    gsl_vector_set(xeq,2,x_eq[2]);

    gsl_blas_dgemv(CblasNoTrans,1.0,R,xeq,0,xec);

    x_ec[0] = gsl_vector_get(xec,0);
    x_ec[1] = gsl_vector_get(xec,1);
    x_ec[2] = gsl_vector_get(xec,2);
}

Ecliptic2Equatorial::Ecliptic2Equatorial(){
    xec = gsl_vector_alloc(3);
    xeq = gsl_vector_alloc(3);
    R = gsl_matrix_alloc(3,3);

    gsl_matrix_set(R,0,0,1);
    gsl_matrix_set(R,0,1,0);
    gsl_matrix_set(R,0,2,0);
    gsl_matrix_set(R,1,0,0);
    gsl_matrix_set(R,1,1,0.917392);
    gsl_matrix_set(R,1,2,-0.397983);
    gsl_matrix_set(R,2,0,0);
    gsl_matrix_set(R,2,1,0.397983);
    gsl_matrix_set(R,2,2,0.917392);
}

Ecliptic2Equatorial::~Ecliptic2Equatorial(){
    if( R != NULL )
        gsl_matrix_free(R);
    if( xec != NULL )
        gsl_vector_free(xec);
    if( xeq != NULL )
        gsl_vector_free(xeq);
}

void Ecliptic2Equatorial::transform(double *x_ec, double *x_eq ){
    gsl_vector_set(xec,0,x_ec[0]);
    gsl_vector_set(xec,1,x_ec[1]);
    gsl_vector_set(xec,2,x_ec[2]);

    gsl_blas_dgemv(CblasNoTrans,1.0,R,xec,0,xeq);

    x_eq[0] = gsl_vector_get(xeq,0);
    x_eq[1] = gsl_vector_get(xeq,1);
    x_eq[2] = gsl_vector_get(xeq,2);
}
