//	define API to call ephemeris functions

#include <iomanip>
#include <cstdlib>
#include <cmath>

#include "ephcom.h"
#include "misc.hpp"

#ifndef _EPHEMERIS_HPP_
#define _EPHEMERIS_HPP_

class Ephemeris{
public:
    FILE *jpl405;

    Ephemeris();
    Ephemeris(std::string& jpl504_filename);
    ~Ephemeris();
    int init(std::string& jpl504_filename);
    void purge();

//  functions to get Sun and Moon's positions at given julian dates,w.s.t earth
    int get_sun_position(double jdate, double&x, double&y, double& z);
    int get_sun_position(double jdate, double&x, double&y, double& z, double& dist);

    int get_sun_position(double jdate, double sun[]);
    int get_sun_position(double jdate, double sun[], double& dist);
    
    int get_moon_position(double jdate, double&x, double&y, double& z);
    int get_moon_position(double jdate, double&x, double&y, double& z, double& dist);

    int get_moon_position(double jdate, double moon[]);
    int get_moon_position(double jdate, double moon[], double& dist);

    void run_jpl405_test(double t_start, double t_end, int steps=100);
};


#endif  // _EPHEMERIS_HPP_

