#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){
    MockOrbit orbit;
    // orbit.set_time_range();
    orbit.set_eccentricity(0.025);
    orbit.set_time_interval(120);
    orbit.set_orbit_num(1.00000005);
    orbit.write_orbit("debug_orbit/1.txt");

    orbit.set_time_interval(3.75);
    // orbit.set_orbit_num(1.05);
    orbit.write_orbit("debug_orbit/mock_orbit_ref.txt");
}
