#include "inspector.hpp"

using namespace std;

Inspector::Inspector(){
    test_cmg_constraint = false;
    test_energy_constraint = false;
    test_solar_panel_orientation = false;

    test_sun_avoidance = false;
    test_moon_avoidance = false;
    test_earth_avoidance = false;
    test_saa_avoidance = false;
}

Inspector::~Inspector(){

}

void Inspector::run_tests(){

//  Sun and Moon gemometrical tests
    test_sun_obs_direction(sim_results.size, 10);
    test_moon_obs_direction(sim_results.size, 10);

//	Sun avoidance test
	if( test_sun_avoidance ){
		cout << "--> testing sun avoidance angle\n";
		test_sun_obs_direction(sim_results.size,10);
	}
//	Moon avoidance test
	if( test_moon_avoidance ){
		cout << "--> testing moon avoidance angle\n";
		test_moon_obs_direction(sim_results.size,10);
	}

//  Earth stray light test
	if( test_earth_avoidance ){
		cout << "--> testing earth avoidance angle (stray-light)\n";
    	test_earth_stray_light(sim_results.size, 10);
	}

//  SAA test
    if( test_saa_avoidance ){
		cout << "--> testing SAA avoidance\n";
		saa.set_area_tolerance(1e-10);
		test_saa_condition(sim_results.size,10);
	}

//  CMG test
	if( test_cmg_constraint ){
		cout << "--> testing CMG constraints\n";
		bool use_5y = true;
		bool use_moving_window = true;
		test_cmg(use_5y, use_moving_window);
	}

//	Solar energy test
	if( test_energy_constraint ){
		cout << "--> testing solar energy condition\n";
		test_energy();
	}

//	Solar panel orientation test
	if( test_solar_panel_orientation ){
		cout << "--> testing solar panel orientation\n";
		test_panel();
	}
}