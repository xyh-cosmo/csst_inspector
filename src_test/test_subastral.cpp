#include "inspector.hpp"

using namespace std;

void matrixMultiple(double a[3][3], double b[3][3], double result[3][3]) {

    if (a == NULL || b == NULL ) {
        return;
    }

    int i, j, k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (k = 0; k < 3; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main( int argc, char* argv[] ){

    if( argc != 3 ){
        cout << "usage:\n" << argv[0] << " t_start t_end \n";
        exit(0);
    }

    double jdate_origin = 2459766;
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


//  get inital Greenwich ra
    double x,y,z,ra0=-1,dec0=-99;
    double t0 = floor(jdate_origin-0.5)+0.5;
    jdate_origin = t0;

    if( jpl405.get_sun_position(t0, x, y, z) == 0 ){
        double x_sun[3] = {x,y,z};
        std::cout.precision(10);
        Cartesian2Spherical_ra_dec_degree(x_sun, ra0, dec0);

        if( ra0 >= 180 )
            ra0 = ra0 - 180;
        else
            ra0 = ra0 + 180;    
    }

    // std::cout.precision(10);
    // std::cout << " ra0 = " << ra0 << ", dec0 = " << dec0 << "\n";

//  
    double orbitAV = 2*M_PI/86400;
    double altitude = R_earth;

    int num=10;
    double jt = jdate_start;
    while( jt <= jdate_end ){
        double x_sat[3];
        double ra=-1,dec=-99;
        if( sat_orbit.get_satellite_position(jt,x_sat) == 0 ){
            Cartesian2Spherical_ra_dec_degree(x_sat,ra,dec);
        }

        double delta_jt = jt - jdate_origin;
        double delta_time = delta_jt*86400.0;
        double f = fmod(orbitAV * delta_time, 2. * M_PI);
        double pa = 0;

        double obX = altitude * cos(f);
        double obY = altitude * sin(f);
        double obZ = 0.;

        double xaa, zaa;
        // double arcInclination = inclination * 2. * PI / 360.;
        double arcInclination = 0.;

    //  变换到赤道惯性坐标系的变换矩阵
        xaa = arcInclination; // = 0
        zaa = fmod(pa + ra0 * 2. * M_PI / 360., 2.*M_PI); // pa=0

        double tranMatrix4[3][3] = { { 1.0, 0.0, 0.0 },
                                    { 0.0, cos(-xaa), sin(-xaa) },
                                    { 0.0,-sin(-xaa), cos(-xaa) } };

        double tranMatrix5[3][3] = { { cos(-zaa), sin(-zaa), 0.0 },
                                    {-sin(-zaa), cos(-zaa), 0.0 },
                                    { 0.0, 0.0, 1.0 } };

        double eMatrix[3][3];
        matrixMultiple(tranMatrix5, tranMatrix4, eMatrix);
        double intertiaX = eMatrix[0][0] * obX + eMatrix[0][1] * obY + eMatrix[0][2] * obZ;
        double intertiaY = eMatrix[1][0] * obX + eMatrix[1][1] * obY + eMatrix[1][2] * obZ;
        double intertiaZ = eMatrix[2][0] * obX + eMatrix[2][1] * obY + eMatrix[2][2] * obZ;

        double tmp[3];
        tmp[0] = intertiaX;
        tmp[1] = intertiaY;
        tmp[2] = intertiaZ;

        double greenWith[2];
        Cartesian2Spherical_ra_dec_degree(tmp,greenWith[0],greenWith[1]);

        // std::cout.precision(10);
        // std::cout << "greenwith[0] = " << greenWith[0] << "\n";
        // std::cout << "greenwith[1] = " << greenWith[1] << "\n";
        // std::cout << "ra  = " << ra << "\n";
        // std::cout << "dec = " << dec << "\n";


        double result[2];
        result[0] = ra  - greenWith[0];
        result[1] = dec;
        
        // std::cout << "result[0] = " << result[0] << "\n";
        // std::cout << "result[1] = " << result[1] << "\n";
        // exit(0);

        if (result[0] > 180.) {
            result[0] = result[0] - 360.;
        }
        if (result[0] < -180.) {
            result[0] = result[0] + 360.;
        }

        std::cout.precision(10);
        std::cout << " Jdate: " << std::setw(15) << jt;
        std::cout << " ra = " << std::setw(14) << result[0]
                  << " dec = " << std::setw(14) << result[1] << "\n";

        jt += (jdate_end-jdate_start)/num;
    }


    if( has_jpl405)
        jpl405.purge();

    return 0;
}
