#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv [] ){
    if( argc != 2 ){
        cout << "usage: " << argv[0] << " orbite_data_dir\n";
        exit(0);
    }

    string dir(argv[1]);
    SatelliteOrbit sat_orbit(dir);

    double t_start = 2459767;
    double t = t_start;

    double dist,x,y,z;
    while( t < t_start + 365.25*10 ){
        if( sat_orbit.get_satellite_position(t,x,y,z,dist) == 0 ){
            cout.precision(10);
            cout << " Yr = " << setw(13) << (t-2459766)/365.25
                 << " x = " << setw(13) << x
                 << " y = " << setw(13) << y
                 << " z = " << setw(13) << z
                 << " dist = " << setw(13) << dist 
                 << " height = " << setw(13) << dist - R_earth
                 << "\n";
        }

        // t += 60./86400;
        t += 0.5;
    }
    

    return 0;
}
