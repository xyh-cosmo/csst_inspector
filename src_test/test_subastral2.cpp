//  检验了张鑫的计算星下点的算法!
//  这里采用的算法,从几何意义上来讲更加直观和容易理解.

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
    // double t0 = floor(jdate_origin-0.5)+0.5;
    double t0 = floor(jdate_origin);    // 这里与张鑫的做法不同,直接以一天的起始时刻为基准点,作为本初子午线的定义.
    jdate_origin = t0;

    if( jpl405.get_sun_position(t0, x, y, z) == 0 ){
        double x_sun[3] = {x,y,z};
        std::cout.precision(10);
        Cartesian2Spherical_ra_dec_degree(x_sun, ra0, dec0);

        // 这里不再需要判断,见上面注释中的说明!
        // if( ra0 >= 180 )
        //     ra0 = ra0 - 180;
        // else
        //     ra0 = ra0 + 180;    
    }

    std::cout.precision(10);
    std::cout << " T0 = " << t0 << "\n";
    std::cout << " ra0 = " << ra0 << ", dec0 = " << dec0 << "\n";

//  
    double orbitAV = 2*M_PI/86400;

    int num=10;
    double jt = jdate_start;
    while( jt <= jdate_end ){
        double delta_jt = jt - jdate_origin;
        double delta_time = delta_jt*86400.0;

        double x_sat[3];
        double ra=-1,dec=-99;
        if( sat_orbit.get_satellite_position(jt,x_sat) == 0 ){
            Cartesian2Spherical_ra_dec_degree(x_sat,ra,dec);
        }

        double d_ra = fmod(orbitAV * delta_time, 2. * M_PI)*180/M_PI;
        double greenWith = ra0 + d_ra;


        double result[2];

        // if( greenWith < 0 ) // this should never happen, since d_ra >= 0
        //     greenWith += 360;

        // if( greenWith > 360 )
        //     greenWith -= 360;

        // result[0] = ra - greenWith;
        // result[1] = dec;
        
        // if (result[0] > 180.) {
        //     result[0] = result[0] - 360.;
        // }
        
        // if (result[0] < -180.) {
        //     result[0] = result[0] + 360.;
        // }

        double x0 = cos(greenWith*M_PI/180);
        double y0 = sin(greenWith*M_PI/180);

        double x1 = cos(ra*M_PI/180);
        double y1 = sin(ra*M_PI/180);

        double cosval = x0*x1+y0*y1;
        double angle = acos(cosval)*180/M_PI;

        double cross = x1*y0-x0*y1;
        angle = angle*(-cross/fabs(cross));

        result[0] = angle;
        result[1] = dec;

        std::cout.precision(10);
        std::cout << " Jdate: " << std::setw(15) << jt;
        std::cout << " -ra  = " << std::setw(14) << result[0]
                  << " -dec = " << std::setw(14) << result[1] << "\n";

        jt += (jdate_end-jdate_start)/num;
    }


    if( has_jpl405)
        jpl405.purge();

    return 0;
}
