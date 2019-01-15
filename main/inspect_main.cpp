#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 3 ){
        cout << "usage: \n" << argv[0] 
             << " jpl_file simulation_result.txt\n"; 
        exit(0);
    }

    string jpl_file(argv[1]);
    string sim_result_file(argv[2]);

    Inspector inspector;

    // inspector.sim_results.load_results(sim_result_file,10000);
    inspector.sim_results.load_results(sim_result_file);
    inspector.ephem.init(jpl_file);

    // inspector.sat_orbit.set_orbit_dir("orbit20160925");
    // inspector.sat_orbit.read_orbits(5);  // read only the first 10 orbits data
    // inspector.sat_orbit.read_orbits(47);  // read all orbits data
    
//  run tests
    inspector.run_tests();

//  Sun and Moon gemometrical tests
//     inspector.test_sun_obs_direction(inspector.sim_results.size, 10);
//     inspector.test_moon_obs_direction(inspector.sim_results.size, 10);

// //  Earth stray light tests
//     inspector.test_earth_stray_light(inspector.sim_results.size, 10);

//  SAA tests
    // inspector.saa.set_area_tolerance(1e-10);
    // inspector.test_saa_condition(inspector.sim_results.size,10);

//  CMG tests
    // bool use_5y = true;
    // bool use_moving_window = true;
    // inspector.test_cmg(use_5y, use_moving_window);
}
