//	This header includes declarations of the class that is used to store the survey simulation results

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "ephemeris_api.hpp"
#include "satellite.hpp"
#include "saa.hpp"

#ifndef _CONTAINER_HPP_
#define _CONTAINER_HPP_

class SimulationResults{
public:
    int size;

    std::vector<double> jdate;
    std::vector<double> ra;
    std::vector<double> dec;

    std::vector<double> sat_x_eq;
    std::vector<double> sat_y_eq;
    std::vector<double> sat_z_eq;

    std::vector<double> sun_x_eq;
    std::vector<double> sun_y_eq;
    std::vector<double> sun_z_eq;

    std::vector<double> moon_x_eq;
    std::vector<double> moon_y_eq;
    std::vector<double> moon_z_eq;

    // double *area_norm, *area_deep;
    std::vector<double> deepflag;
    std::vector<int> in_galactic_disk;
    std::vector<double> exp_time;
    std::vector<double> tAngle;
    std::vector<int> in_sun_side;
    std::vector<double> cmg;
    std::vector<double> battery;
    std::vector<double> sun_panel_angle;
    std::vector<double> saa_time;
    std::vector<int> sky_id;
    // std::vector<int> steps_5deg;
    
    SimulationResults();
    SimulationResults(std::string& result, int num=-1);
    ~SimulationResults();

    void load_results(std::string& result, int num=-1);
    void print_summary(int num=-1);
    void exposure_info(int i);
};

void combine_drift( std::string& sim_result_root, 
					Ephemeris& jpl, 
					SatelliteOrbit& sat_orbit,
					SAA_Condition& saa );

#endif // _CONTAINER_HPP_