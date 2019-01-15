#include "inspector.hpp"

int Inspector::test_saa_condition( int first_n, int chk_per_obs ){
	std::cout << "--> calling " << __FUNCTION__ << " to check SAA conditions\n";

	int fail_cnt=0;
	int nmax = first_n;

	if( chk_per_obs < 2 ){
        std::cout << "\n## Error in calling function: " << __FUNCTION__ 
                  << ", chk_per_obs should be greater than one!\n\n";
        exit(0);
    }

	for( int i=0; i<nmax; ++i ){
		double jt_start = sim_results.jdate[i];
		double jt_end = jt_start + sim_results.exp_time[i]/86400;
		double ra_ref,dec_ref;

		double jt = jt_start;
		double djt = (jt_end-jt_start)/chk_per_obs;
		
		while( jt <= jt_end ){
			sat_orbit.get_subastral_position(jt,ra_ref,dec_ref);

			if( saa.check(ra_ref,dec_ref) != 0 ){
				fail_cnt++;
				break;
			}

			jt += djt;
		}
	}

	if( fail_cnt == 0 ){
		std::cout << "> SAA test result: all pass!\n";
	} else {
		std::cout << "> SAA test result: fail_cnt = " << fail_cnt << "\n";
	}

	return fail_cnt;
}