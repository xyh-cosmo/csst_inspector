#include "inspector.hpp"

using namespace std;

int main( int argc, char * argv[] ){

	if( argc != 3 ){
		cout << "usage:\n" << argv[0] << " orbit_dir sim_result_root\n";
		exit(0);
	}

//	JPL
	Ephemeris jpl405;
	string jpl_fname = "jpl.405";
	jpl405.init(jpl_fname);

//	Satellite orbit data
	string orbit_dir(argv[1]);
	SatelliteOrbit sat_orbit(orbit_dir);

//	SAA
	SAA_Condition saa;

//	Now lets combine the simulation results
	string root(argv[2]);

	combine_drift( root, jpl405, sat_orbit, saa );

//	clean JPL data
	jpl405.purge();

	return 0;
}