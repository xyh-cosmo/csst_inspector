#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv [] ){
    if( argc != 2 ){
        cout << "usage: " << argv[0] << " orbite_data_dir\n";
        exit(0);
    }

    string dir(argv[1]);
    SatelliteOrbit sat_orbit(dir);

    double t_start = 2459766;
    double t = t_start;

    double dist,x,y,z;

//    time_t T_start = time(nullptr);
	time_t T_start = time(NULL);

    while( t < t_start + 365.25*10 ){
        if( sat_orbit.get_satellite_position(t,x,y,z,dist) == 0 ){
            // cout.precision(10);
            // cout << " Yr = " << setw(15) << (t-2459766)/365.25
            //      << " x = " << setw(15) << x
            //      << " y = " << setw(15) << y
            //      << " z = " << setw(15) << z
            //      << " dist = " << setw(15) << dist 
            //      << "\n";
        }

        t += 1./86400;
        // t += 10;
    }
    
//    time_t T_end = time(nullptr);
	time_t T_end = time(NULL);

    cout << "time used: " << (T_end-T_start) << " seconds\n";

    return 0;
}
