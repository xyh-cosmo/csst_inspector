#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 3 ){
        cout << "usage:\n" << argv[0] << " jt_start jt_end \n";
        exit(0);
    }

//  Load JPL405 data
    bool has_jpl405 = false;
    Ephemeris jpl405;
    if( argc == 3 ){
        string jpl405_filename("jpl.405");
        if( jpl405.init(jpl405_filename) == 0 ){
            has_jpl405 = true;
        }
    }

    double x,y,z;
    double jdate_start = atof(argv[1]);
	double jdate_end   = atof(argv[2]);
	
	double dt = 0.5;	// 0.5 day
	double jdate = jdate_start;
	
	ofstream outf("sun_orbit.txt");
	while( jdate <= jdate_end ){
		if( jpl405.get_sun_position(jdate, x, y, z) == 0){
			outf.precision(15);
			outf << setw(18) << x << " "
				 << setw(18) << y << " "
			     << setw(18) << z << "\n";
		}
		jdate = jdate + dt;
	}

	outf.close();

    if( has_jpl405 ){
        jpl405.purge();
    }

    return 0;
}
