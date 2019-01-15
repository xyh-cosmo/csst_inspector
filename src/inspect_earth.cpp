#include "inspector.hpp"

int Inspector::test_earth_stray_light(int first_n, int chk_per_obs){

    std::cout << "\n--> calling " << __FUNCTION__ << " to check angles between obs-pointing and earth stray light\n";

    int fail_cnt=0;
    int sunside_cnt=0;
    int orbit_err_cnt=0;
    int nmax = first_n;

    if( chk_per_obs < 2 ){
        std::cout << "\n## Error in calling function: " << __FUNCTION__ 
                  << ", chk_per_obs should be greater than one!\n\n";
        exit(0);
    }

    double max_diff = 0;
    double theta_dark = 30;
    double theta_light = 70;

    for( int i=0; i<nmax; ++i ){

        double t_start = sim_results.jdate[i];
        double t_end = t_start + sim_results.exp_time[i]/86400;
        double dt = (t_end-t_start)/(chk_per_obs-1);

        double x_sky[3];
        double x_sat_eq[3], x_sat_ec[3];
        double x_sun_eq[3], x_sun_ec[3];
        double dist_sat, dist_sun;

        Spherical2Cartesian_ra_dec_degree(sim_results.ra[i],sim_results.dec[i],x_sky);

        int j=0;
        while( j < chk_per_obs ){
            
            double t = t_start + j*dt;
            
            ephem.get_sun_position(t, x_sun_eq[0], x_sun_eq[0], x_sun_eq[0], dist_sun);
            eq2ec.transform(x_sun_eq,x_sun_ec);

            int status = -999;
            double diff = 0;
            if( sat_orbit.get_satellite_position(t, x_sat_eq[0], x_sat_eq[1], x_sat_eq[2], dist_sat) == 0 ){
            
                eq2ec.transform(x_sat_eq,x_sat_ec);

                double cosval = ( x_sat_ec[0]*x_sky[0]
                                + x_sat_ec[1]*x_sky[1]
                                + x_sat_ec[2]*x_sky[2] ) / dist_sat;

                double theta_1 = acos(R_earth/dist_sat)*180/M_PI;  // 地球的遮挡角
                double theta_z = acos(cosval)*180/M_PI; // 观测目标的天顶距

                int flag_sunside = in_sunside(x_sun_ec, x_sat_ec);
                
                //  case 1: 未被太阳照射,此时任何方向都可以用暗边角条件
                if( flag_sunside == 0 ){
                    if( theta_z < 180-theta_1-theta_dark ){
                        j++;
                        continue;
                    }
                    else
                        status = 1;
                }

                //  case 2: 卫星位于阳照区且只能看到阳照区
                double cosval_sat_sun = ( x_sun_ec[0]*x_sky[0]
                                        + x_sun_ec[1]*x_sky[1]
                                        + x_sun_ec[2]*x_sky[2] ) / dist_sat / dist_sun;
                
                if( cosval_sat_sun > cos((90-theta_1)*M_PI/180) ){
                    if( theta_z < 180-theta_1-theta_light ){
                        j++;
                        continue;
                    }
                    else
                        status = 2;
                }

                // case 3: 卫星能同时看到部分阴影区和阳照区。
                // 如果上面两种情况都不满足，剩下的情况必然属于这里所讨论的case 3
                // TODO:这里的模型需要进一步改进！
                {
                    if( theta_z < 180-theta_1-theta_light ){
                        j++;
                        continue;
                    }
                    else{
                        if( theta_z < (180-theta_1-theta_dark) ){
                            double xC[3],xB[3],xQ[3];
                            double L_OC = R_earth*R_earth/dist_sat;
                            double L_QC = sqrt(R_earth*R_earth-L_OC*L_OC);
                            double L_OB = L_OC/cosval;

                            xC[0] = x_sat_ec[0]*L_OC/dist_sat;
                            xC[1] = x_sat_ec[1]*L_OC/dist_sat;
                            xC[2] = x_sat_ec[2]*L_OC/dist_sat;

                            xB[0] = x_sky[0]*L_OB;
                            xB[1] = x_sky[1]*L_OB;
                            xB[2] = x_sky[2]*L_OB;

                            double L_CB = sqrt( (xC[0]-xB[0])*(xC[0]-xB[0]) 
                                              + (xC[1]-xB[1])*(xC[1]-xB[1]) 
                                              + (xC[2]-xB[2])*(xC[2]-xB[2]) );

                            xQ[0] = xC[0] + (L_QC/L_CB)*(xB[0]-xC[0]);
                            xQ[1] = xC[1] + (L_QC/L_CB)*(xB[1]-xC[1]);
                            xQ[2] = xC[2] + (L_QC/L_CB)*(xB[2]-xC[2]);

                            double cosval_Q_sun = ( xQ[0]*x_sun_ec[0] 
                                                  + xQ[1]*x_sun_ec[1] 
                                                  + xQ[2]*x_sun_ec[2] ) / R_earth / dist_sun;

                            // 判断Q点是否处在阴影区
                            if( cosval_Q_sun < cos(0.5*M_PI+(16+35.4*2)/60) ){
                                // std::cout << "OK-4 ...\n";
                                j++;
                                continue;
                            }
                            else
                                status = 3;
                        }
                        else{
                            status = 4;
                        }
                    }
                    
                }

                switch (status){
                    case 1: 
                        diff = std::fabs( theta_z - (180-theta_1-theta_dark) );
                        break;
                    case 2: 
                        diff = std::fabs( theta_z - (180-theta_1-theta_light) );
                        break;
                    case 3: 
                        PrintWarning("status = 3");
                        break;
                    case 4: 
                        PrintWarning("status = 4");
                        break;
                    default: 
                        // PrintError(" unknown value " + std::to_string(status) + " for status");
                        PrintWarning(" unknown value " + Int2String(status) + " for status");
                        break;
                }
            } else {
                PrintWarning("failed to get satllite position, I = " + Int2String(i) + " .");
                status = -1;
                orbit_err_cnt++;
            }
                
            if( status != 0 ){
                std::cout << " diff_" << status << " = " << diff << "\n";
                max_diff = max_diff < diff ? diff : max_diff;
                fail_cnt++;
                break;
            }
            
            j++;
        }
    }

    if( fail_cnt == 0 ){
        std::cout << "> Earth horizon & stray light test result: all pass! \n";
    } else {
        std::cout << "> Earth horizon test result: fail_cnt = " << fail_cnt << "\n"
                  << "> sunside_cnt = " << sunside_cnt << "\n"
                  << "> max_diff = " << max_diff << "\n"
                  << "> orbit_err_cnt = " << orbit_err_cnt << "\n";
    }

    return fail_cnt;
}
