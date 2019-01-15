#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv [] ){
    if( argc != 2 ){
        cout << "usage: " << argv[0] << " orbite_data_dir\n";
        exit(0);
    }

    string dir(argv[1]);
    SatelliteOrbit sat_orbit(dir);

    // for( size_t i=1; i<=45; i++ ){
    //     int status = sat_orbit.read_orbit_file(i);
    //     if( status == 0 ){
    //         cout << "--> sucessfully read " << dir << "/" + to_string(i) + ".txt" << "\n";
    //         cout << "--> data_size: " << sat_orbit.get_size() << "\n";
    //         cout.precision(15);
    //         cout << " @t_start = " << setw(18) << sat_orbit.get_t_start() << "\n"
    //              << " @t_end   = " << setw(18) << sat_orbit.get_t_end() << "\n";
    //     }
    //     else if( status == 1 ){
    //         cout << "failed to read data from: " << dir << "/" + to_string(i) + ".txt" << "\n";
    //     }
    // }


    double t_start = 2459767;
    double t = t_start;

    double dist,x,y,z;
    while( t < t_start + 365.25*10 ){
        if( sat_orbit.get_satellite_position(t,x,y,z,dist) == 0 ){
            cout.precision(10);
            cout << " Yr = " << setw(15) << (t-2459766)/365.25
                 << " x = " << setw(15) << x
                 << " y = " << setw(15) << y
                 << " z = " << setw(15) << z
                 << " dist = " << setw(15) << dist 
                 << "\n";
        }

        t += 60./86400;
        // t += 0.5;
    }
    

    return 0;
}
