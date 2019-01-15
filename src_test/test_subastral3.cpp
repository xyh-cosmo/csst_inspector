//  检验了张鑫的计算星下点的算法!
//  这里采用的算法,从几何意义上来讲更加直观和容易理解.

#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 3 ){
        cout << "usage:\n" << argv[0] << " t_start t_end \n";
        exit(0);
    }

    // double jdate_origin = 2459766;
    double jdate_start = std::atof(argv[1]);
    double jdate_end = std::atof(argv[2]);

//  JPL data
    bool has_jpl405 = false;
    Ephemeris jpl405;
    string jpl405_filename("jpl.405");
    if( jpl405.init(jpl405_filename) == 0 ){
        has_jpl405 = true;
    }

//  Sat orbit data
    SatelliteOrbit sat_orbit;
    sat_orbit.set_orbit_dir("orbit20160925/");
    sat_orbit.read_orbits(2);
    sat_orbit.init_subastral(jpl405,2459766);

    int num=10;
    double jt = jdate_start;
    while( jt <= jdate_end ){
        double ra_ref,dec_ref;

        sat_orbit.get_subastral_position(jt,ra_ref,dec_ref);

        std::cout.precision(10);
        std::cout << " Jdate: " << std::setw(15) << jt;
        std::cout << " -ra  = " << std::setw(14) << ra_ref
                  << " -dec = " << std::setw(14) << dec_ref << "\n";

        jt += (jdate_end-jdate_start)/num;
    }


    if( has_jpl405)
        jpl405.purge();

    return 0;
}
