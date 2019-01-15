#include "inspector.hpp"

int Inspector::test_moon_obs_direction(int first_n, int chk_per_obs){
    
    std::cout << "\n--> calling " << __FUNCTION__ << " to check angles between obs-pointing and Moon\n";
    
    int fail_cnt=0;
    int nmax = first_n;

    if( chk_per_obs < 2 ){
        std::cout << "\n## Error in calling function: " << __FUNCTION__ 
                  << ", chk_per_obs should be greater than one!\n\n";
        exit(0);
    }

    double max_diff=0;
    for( int i=0; i<nmax; ++i ){
        double t_start = sim_results.jdate[i];
        double t_end = t_start + sim_results.exp_time[i]/86400;

        int j=0;
        while( j < chk_per_obs ){

            double x_sky[3];
            double x_sat_eq[3], x_sat_ec[3];
            double x_moon_eq[3], x_moon_ec[3];
            
            double t = t_start + j*(t_end-t_start)/(chk_per_obs-1);
            ephem.get_moon_position(t,
                                    x_moon_eq[0],
                                    x_moon_eq[1],
                                    x_moon_eq[2]);
            eq2ec.transform(x_moon_eq,x_moon_ec);
            
            double ra = sim_results.ra[i];
            double dec= sim_results.dec[i];
            Spherical2Cartesian_ra_dec_degree(ra,dec,x_sky);

            sat_orbit.get_satellite_position(t, x_sat_eq[0], x_sat_eq[1], x_sat_eq[2]);
            eq2ec.transform(x_sat_eq,x_sat_ec);

            // double x_sat2moon[3] = {x_moon_ec[0]-x_sat_ec[0]*0, 
            //                         x_moon_ec[1]-x_sat_ec[1]*0, 
            //                         x_moon_ec[2]-x_sat_ec[2]*0};

            double x_sat2moon[3] = {x_moon_ec[0]-x_sat_ec[0], 
                                    x_moon_ec[1]-x_sat_ec[1], 
                                    x_moon_ec[2]-x_sat_ec[2]};

            double cosval = x_sat2moon[0]*x_sky[0]
                          + x_sat2moon[1]*x_sky[1]
                          + x_sat2moon[2]*x_sky[2];

            double dist_sat2moon = sqrt(  x_sat2moon[0]*x_sat2moon[0]
                                        + x_sat2moon[1]*x_sat2moon[1]
                                        + x_sat2moon[2]*x_sat2moon[2] );
            cosval /= dist_sat2moon;

            double cos40deg = cos(40./180*M_PI);
            if( cosval > cos40deg ){
                double diff = std::fabs(acos(cosval)*180/M_PI-40);
                max_diff = max_diff < diff ? diff : max_diff;
                fail_cnt++;
                break;
            }


            j++;
        }
            
        
    }

    std::cout << "> Moon obs-direction test result: fail_cnt = " << fail_cnt << "\n"
              << "> max_diff = " << max_diff << " degs\n";
    
    return fail_cnt;
}