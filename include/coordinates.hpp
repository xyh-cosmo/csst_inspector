// TODO: combine all the functions and classes into class Coordinates

#include "GSL_func.hpp"

#ifndef _COORDINATES_HPP_
#define _COORDINATES_HPP_

//  convert Cartesian coordinates into 2D spherical coordinates
void Cartesian2Spherical_ra_dec_radian(double* X, double& ra, double& dec);
void Cartesian2Spherical_ra_dec_degree(double* X, double& ra, double& dec);
void Cartesian2Spherical_theta_phi_radian(double* X, double& theta, double& phi);
void Cartesian2Spherical_theta_phi_degree(double* X, double& theta, double& phi);

//  the following 4 are used to test the above 4
void Spherical2Cartesian_ra_dec_radian(double ra, double dec, double x[]);
void Spherical2Cartesian_ra_dec_degree(double ra, double dec, double x[]);
void Spherical2Cartesian_theta_phi_radian(double theta, double phi, double x[]);
void Spherical2Cartesian_theta_phi_degree(double theta, double phi, double x[]);

//  transform equatorial coordinates to ecliptical coordinates

class Equatorial2Ecliptic{
private:
    gsl_matrix *R;
    gsl_vector *xeq, *xec;
public:
    Equatorial2Ecliptic();
    ~Equatorial2Ecliptic();
    void transform(double *x_eq, double *x_ec );
};

class Ecliptic2Equatorial{
private:
    gsl_matrix *R;
    gsl_vector *xec, *xeq;
public:
    Ecliptic2Equatorial();
    ~Ecliptic2Equatorial();
    void transform(double *x_ec, double *x_eq );
};

#endif