#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 2 ){
        cout << "usage:\n" << argv[0] << " julia_date \n";
        exit(0);
    }

    double jdate = std::atof(argv[1]);

//  Load JPL405 data
    bool has_jpl405 = false;
    Ephemeris jpl405;
    string jpl405_filename("jpl.405");
    if( jpl405.init(jpl405_filename) == 0 ){
        has_jpl405 = true;
    }

    double x,y,z;
	if( jpl405.get_sun_position(jdate, x, y, z) == 0 ){
        double x_sun[3] = {x,y,z};
        double ra, dec;
        Cartesian2Spherical_ra_dec_degree(x_sun, ra, dec);
        std::cout.precision(12);
        std::cout << " Jdate: " << jdate << "\n"
                  << " x   = " << std::setw(14) << x << "\n"
                  << " y   = " << std::setw(14) << y << "\n"
                  << " z   = " << std::setw(14) << z << "\n";

        std::cout.precision(8);
        std::cout << " ra  = " << std::setw(14) << ra << "\n"
                  << " dec = " << std::setw(14) << dec << "\n";
    }

    if( has_jpl405)
        jpl405.purge();

    return 0;
}
