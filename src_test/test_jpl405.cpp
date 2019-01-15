#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 3 ){
        cout << "usage:\n" << argv[0] << " jpl405 julian_date\n";
        exit(0);
    }

//  Load JPL405 data
    bool has_jpl405 = false;
    Ephemeris jpl405;
    if( argc == 3 ){
        string jpl405_filename(argv[1]);
        if( jpl405.init(jpl405_filename) == 0 ){
            has_jpl405 = true;
            // jpl405.run_jpl405_test(2459766.,2459866.,1000);
        }
    }

    double x,y,z;
    double jdate = atof(argv[2]);

    if( jpl405.get_sun_position(jdate, x, y, z) == 0){
        cout.precision(15);
        cout << "Sun:\n"
             << setw(18) << x << " "
             << setw(18) << y << " "
             << setw(18) << z << "\n";
    }

    if( jpl405.get_moon_position(jdate, x, y, z) == 0){
        cout.precision(15);
        cout << "Moon:\n"
             << setw(18) << x << " "
             << setw(18) << y << " "
             << setw(18) << z << "\n";
    }

    if( has_jpl405 ){
        jpl405.purge();
    }

    return 0;
}
