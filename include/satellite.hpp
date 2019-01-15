//  Get equatorial coordinates of the satllite (telescope) at any given time
#include "ephemeris_api.hpp"
#include "GSL_func.hpp"


#ifndef _SATELLITE_HPP_
#define _SATELLITE_HPP_


struct OrbitData{
    int size;
    double *t;
    double *x, *y, *z;
#if defined(_INCLUDE_VELOCITY_)
    double *vx, *vy, *vz;
#endif
    double t_start, t_end;

    OrbitData();
    ~OrbitData();
};


class SatelliteOrbit{
private:
    std::string orbit_data_dir;
    int orbit_num;
    int current_orbit_idx;    
    double current_t_start, current_t_end;

// t_head is the head time of the first orbit data, t_tail is the end of the last orbit data
    double t_head, t_tail;

    std::vector<OrbitData *> orbits; 
    std::vector<gsl_interp_accel *> accel;

    bool lagrange_interp;

//  for determining the subastral position of Satellite
    bool status_subastral;
    double ra0_greenwich;
    double jdate0;
public:
    SatelliteOrbit();
    SatelliteOrbit(std::string data_dir);
    ~SatelliteOrbit();

    void use_lagrange_interp(bool opt);
    void set_orbit_dir(std::string data_dir);
    int read_one_orbit(int idx);
    void read_orbits();
    void read_orbits(int num);

    int get_satellite_position( double jdate, double& xt, double& yt, double& zt, double& dist );
    int get_satellite_position( double jdate, double& xt, double& yt, double& zt );
    int get_satellite_position( double jdate, double x[], double& dist );
    int get_satellite_position( double jdate, double x[] );

    int get_satellite_velocity( double jdate, double& vx, double& vy, double& vz );
    int get_satellite_velocity( double jdate, double v[] );

    void init_subastral( Ephemeris& jpl405, double jd0 );
    void get_subastral_position( double jdate, double& ra, double& dec) ;
};


//  generate mock orbit data for testing the interpolation methods used in class SatlliteOrbit
//  the initial position is (-(a-c),0), and the rotation is anti-clockwise
class MockOrbit{
private:
    double e,a,b,c,k;
    double T,dt,tmax;
    double orbit_num;
    // std::string ofilename;
public:
    int (*ode)(double t, const double y[], double dydt[], void* params);
    MockOrbit();
    ~MockOrbit();
    
    void set_eccentricity(double e=0.05);
    void set_period(double T);
    void set_orbit_num(double num);
    void set_time_interval(double dt);
    void get_initial_condition( double& x, double& y, double& vx, double& vy);
    void write_orbit(std::string filename);
};

int ODE_orbit( double t, const double y[], double dydt[], void* params);


#endif // _SATELLITE_HPP_

