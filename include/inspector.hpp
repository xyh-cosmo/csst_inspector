#include <iostream>
#include <cmath>
#include <iomanip>
#include <ctime>

#include "GSL_func.hpp"

#include "container.hpp"
#include "saa.hpp"
#include "satellite.hpp"
#include "solar_panel.hpp"
#include "cmg.hpp"

#include "rotation.hpp"
#include "coordinates.hpp"
#include "sun.hpp"
#include "moon.hpp"
#include "earth.hpp"

#include "gnulliver.h"
#include "ephcom.h"
#include "ephemeris_api.hpp"

#include "misc.hpp"


#ifndef _INSPECTOR_HPP_
#define _INSPECTOR_HPP_

#define R_earth 6371.0  // km

class Inspector{

public:

//  Simulation results
    SimulationResults sim_results;

//  Ephemeris
    Ephemeris ephem;

//  Satllite orbits
    SatelliteOrbit sat_orbit;

//  Equatorial2Ecliptic
    Equatorial2Ecliptic eq2ec;

//  test options
    bool test_cmg_constraint;
    bool test_energy_constraint;
    bool test_solar_panel_orientation;

    bool test_sun_avoidance;
    bool test_moon_avoidance;
    bool test_earth_avoidance;
    bool test_saa_avoidance;


//  CMG_use, CMG_Node, CMG_List
    // CMG_use  cmg_use;
    // CMG_Node cmg_node;
    CMG_List cmg_list;

//  SAA
    SAA_Condition saa;

//  Solar Panel
    SolarPanel solar_panel;

//  constructor and destructor
    Inspector();
    ~Inspector();

//  dynamical tests
    int test_cmg( bool use_5y, bool moving_window );
    int test_energy();
    int test_panel();

//  geometrical tests
    int in_sunside(double x_sun_ec[], double x_sat_ec[]);

    int test_sun_obs_direction(int first_n, int chk_per_obs=10);
    int test_moon_obs_direction(int first_n, int chk_per_obs=10);
    int test_earth_stray_light(int first_n, int chk_per_obs=10);
    int test_saa_condition(int first_n, int chk_per_obs=10);

    void run_tests();
};


#endif // _INSPECTOR_HPP_
