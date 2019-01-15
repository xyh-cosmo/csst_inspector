#include "inspector.hpp"

int Inspector::in_sunside(double x_sun_ec[], double x_sat_ec[]){
    double dist_sun = sqrt(   x_sun_ec[0]*x_sun_ec[0] 
                            + x_sun_ec[1]*x_sun_ec[1] 
                            + x_sun_ec[2]*x_sun_ec[2] );

    double dist_sat = sqrt(   x_sat_ec[0]*x_sat_ec[0] 
                            + x_sat_ec[1]*x_sat_ec[1] 
                            + x_sat_ec[2]*x_sat_ec[2] );

    double cosval0 = ( x_sun_ec[0]*x_sat_ec[0]
                     + x_sun_ec[1]*x_sat_ec[1]
                     + x_sun_ec[2]*x_sat_ec[2] )/dist_sat/dist_sun;
    
    double theta0 = acos(cosval0)*180/M_PI;
    double theta1 = acos(R_earth/dist_sun)*180/M_PI;
    double theta2 = acos(R_earth/dist_sat)*180/M_PI;
    double theta3 = (16+35.4*2)/60;

    return int(theta0 <= (theta1+theta2+theta3));
}

int Inspector::test_sun_obs_direction(int first_n, int chk_per_obs){

    std::cout << "--> calling " << __FUNCTION__ << " to check angles between obs-pointing and Sun\n";

    int fail_cnt=0, sunside_cnt=0;;
    int nmax = first_n;

    if( chk_per_obs < 2 ){
        std::cout << "\n## Error in calling function: " << __FUNCTION__ 
                  << ", chk_per_obs should be greater than one!\n\n";
        exit(0);
    }

    double max_diff = 0;

    for( int i=0; i<nmax; ++i ){
        
        double t_start = sim_results.jdate[i];
        double t_end = t_start + sim_results.exp_time[i]/86400;
        
        if( sim_results.in_sun_side[i] == 1 ){
            sunside_cnt++;
            int j=0;
            while( j < chk_per_obs ){
                double x_sky[3];
                double x_sat_eq[3], x_sat_ec[3];
                double x_sun_eq[3], x_sun_ec[3];
                
                double t = t_start + j*(t_end-t_start)/(chk_per_obs-1);
                ephem.get_sun_position(t, x_sun_eq);
                eq2ec.transform(x_sun_eq,x_sun_ec);

                double ra = sim_results.ra[i];
                double dec= sim_results.dec[i];
                Spherical2Cartesian_ra_dec_degree(ra,dec,x_sky);
                
                sat_orbit.get_satellite_position(t, x_sat_eq);
                eq2ec.transform(x_sat_eq,x_sat_ec);

                double x_sat2sun[3] = { x_sun_ec[0]-x_sat_ec[0], 
                                        x_sun_ec[1]-x_sat_ec[1], 
                                        x_sun_ec[2]-x_sat_ec[2] };

                double cosval = x_sat2sun[0]*x_sky[0]
                              + x_sat2sun[1]*x_sky[1]
                              + x_sat2sun[2]*x_sky[2];

                double dist_sat2sun = sqrt(  x_sat2sun[0]*x_sat2sun[0]
                                           + x_sat2sun[1]*x_sat2sun[1]
                                           + x_sat2sun[2]*x_sat2sun[2] );
                cosval /= dist_sat2sun;

                double cos50deg = cos(50./180*M_PI);
                if( cosval > cos50deg ){
                    double diff = std::fabs(acos(cosval)*180/M_PI-50);
                    max_diff = max_diff < diff ? diff : max_diff;
                    fail_cnt++;
                    break;
                }

                j++;
            }
        }
    }

    std::cout << "> Sun obs-direction test result: fail_cnt = " << fail_cnt << "\n"
              << "> sunside_cnt = " << sunside_cnt << "\n"
              << "> max_diff = " << max_diff << " degs\n";

    return fail_cnt;
}