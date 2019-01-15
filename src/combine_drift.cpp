#include "container.hpp"

using namespace std;

void combine_drift( std::string& sim_result_root, 
					Ephemeris& jpl, 
					SatelliteOrbit& sat_orbit,
					SAA_Condition& saa ){

	string fname1 = sim_result_root + ".dat";
	string fname2 = sim_result_root + "_drift.dat";
	string fname3 = sim_result_root + "_combined.dat";

	SimulationResults sim_result(fname1);
	SimulationResults sim_result_drift(fname2);


	ofstream combined_result(fname3.c_str());

	int idx1=0, idx2=0;
	int cnt_total=0;

	int cnt1=0,cnt2=0;

	int idx1_max = sim_result.size;
	int idx2_max = sim_result_drift.size;
	int status1=0;
	int status2=0;

	double jdate1=0, jdate2=0;
	while( true ){

		if( idx1 < idx1_max ){
			jdate1 = sim_result.jdate[idx1];
			status1 = 0;
		} else {
			status1 = 1;
		}
		
		if( idx2 < idx2_max ){
			jdate2 = sim_result_drift.jdate[idx2];
			status2 = 0;
		} else {
			status2 = 1;
		}

		if( status1 == 0 && status2 == 0 ){

			if( jdate1 < jdate2 ){
				combined_result.precision(16);
				combined_result << setw(16) << jdate1 << " ";

				combined_result.precision(10);
				combined_result << setw(10) << sim_result.dec[idx1] << " "
								<< setw(10) << sim_result.ra[idx1] << " "
								<< setw(14) << sim_result.sat_x_eq[idx1] << " "
								<< setw(14) << sim_result.sat_y_eq[idx1] << " "
								<< setw(14) << sim_result.sat_z_eq[idx1] << " "
								<< setw(14) << sim_result.sun_x_eq[idx1] << " "
								<< setw(14) << sim_result.sun_y_eq[idx1] << " "
								<< setw(14) << sim_result.sun_z_eq[idx1] << " "
								<< setw(14) << sim_result.moon_x_eq[idx1] << " "
								<< setw(14) << sim_result.moon_y_eq[idx1] << " "
								<< setw(14) << sim_result.moon_z_eq[idx1] << " "
								<< setw(5) << 0.0 << " "
								<< setw(5) << 0.0 << " "
								<< setw(10) << sim_result.deepflag[idx1] << " "
								<< setw(5) << sim_result.in_galactic_disk[idx1] << " "
								<< setw(10) << sim_result.exp_time[idx1] << " "
								<< setw(14) << sim_result.tAngle[idx1] << " "
								<< setw(5) << sim_result.in_sun_side[idx1] << " "
								<< setw(10) << sim_result.cmg[idx1] << " "
								<< setw(10) << sim_result.battery[idx1] << " "
								<< setw(10) << sim_result.sun_panel_angle[idx1] << " "
								<< setw(10) << sim_result.saa_time[idx1] << " "
								<< setw(10) << sim_result.sky_id[idx1] 
								<< "\n";
				if( idx1 <= idx1_max ){
					idx1++;
					cnt1++;
					cnt_total++;
				}
			} else if( jdate1 > jdate2 ) {
				double sat_eq[3], sun_eq[3], moon_eq[3];
				if( sat_orbit.get_satellite_position(jdate2,sat_eq) == 0 ){
					jpl.get_sun_position(jdate2,sun_eq[0],sun_eq[1],sun_eq[2]);
					jpl.get_moon_position(jdate2,moon_eq[0],moon_eq[1],moon_eq[2]);

					combined_result.precision(16);
					combined_result << setw(16) << jdate2 << " ";

					combined_result.precision(10);
					combined_result << setw(10) << sim_result_drift.dec[idx2] << " "
									<< setw(10) << sim_result_drift.ra[idx2] << " "
									<< setw(14) << sat_eq[0] << " "
									<< setw(14) << sat_eq[1] << " "
									<< setw(14) << sat_eq[2] << " "
									<< setw(14) << sun_eq[0] << " "
									<< setw(14) << sun_eq[1] << " "
									<< setw(14) << sun_eq[2] << " "
									<< setw(14) << moon_eq[0] << " "
									<< setw(14) << moon_eq[1] << " "
									<< setw(14) << moon_eq[2] << " "
									<< setw(5) << 0.0 << " "
									<< setw(5) << 0.0 << " "
									<< setw(10) << sim_result_drift.deepflag[idx2] << " "
									<< setw(5) << sim_result_drift.in_galactic_disk[idx2] << " "
									<< setw(10) << sim_result_drift.exp_time[idx2] << " "
									<< setw(14) << sim_result_drift.tAngle[idx2] << " "
									<< setw(5) << sim_result_drift.in_sun_side[idx2] << " "
									<< setw(10) << sim_result_drift.cmg[idx2] << " "
									<< setw(10) << sim_result_drift.battery[idx2] << " "
									<< setw(10) << sim_result_drift.sun_panel_angle[idx2] << " "
									<< setw(10) << sim_result_drift.saa_time[idx2] << " "
									<< setw(10) << sim_result_drift.sky_id[idx2] 
									<< "\n";
				} else {
					// PrintWarning("failed to get satellite position!");
					cout << "## jdate2 = " << jdate2 << "\n";
					cout << "## cnt2   = " << cnt2 << "\n";
					PrintError("failed to get satellite position!");
				}

				if( idx2 <= idx2_max ){
					idx2++;
					cnt2++;
					cnt_total++;
				}
			}
		} else {

			if( status1 == 0 ){
				combined_result.precision(16);
				combined_result << setw(16) << jdate1 << " ";

				combined_result.precision(10);
				combined_result << setw(10) << sim_result.dec[idx1] << " "
								<< setw(10) << sim_result.ra[idx1] << " "
								<< setw(14) << sim_result.sat_x_eq[idx1] << " "
								<< setw(14) << sim_result.sat_y_eq[idx1] << " "
								<< setw(14) << sim_result.sat_z_eq[idx1] << " "
								<< setw(14) << sim_result.sun_x_eq[idx1] << " "
								<< setw(14) << sim_result.sun_y_eq[idx1] << " "
								<< setw(14) << sim_result.sun_z_eq[idx1] << " "
								<< setw(14) << sim_result.moon_x_eq[idx1] << " "
								<< setw(14) << sim_result.moon_y_eq[idx1] << " "
								<< setw(14) << sim_result.moon_z_eq[idx1] << " "
								<< setw(5) << 0.0 << " "
								<< setw(5) << 0.0 << " "
								<< setw(10) << sim_result.deepflag[idx1] << " "
								<< setw(5) << sim_result.in_galactic_disk[idx1] << " "
								<< setw(10) << sim_result.exp_time[idx1] << " "
								<< setw(14) << sim_result.tAngle[idx1] << " "
								<< setw(5) << sim_result.in_sun_side[idx1] << " "
								<< setw(10) << sim_result.cmg[idx1] << " "
								<< setw(10) << sim_result.battery[idx1] << " "
								<< setw(10) << sim_result.sun_panel_angle[idx1] << " "
								<< setw(10) << sim_result.saa_time[idx1] << " "
								<< setw(10) << sim_result.sky_id[idx1] 
								<< "\n";
				if( idx1 <= idx1_max ){
					idx1++;
					cnt1++;
					cnt_total++;
				}
			}

			if( status2 == 0 ){
				double sat_eq[3], sun_eq[3], moon_eq[3];
				if( sat_orbit.get_satellite_position(jdate2,sat_eq) == 0 ){
					jpl.get_sun_position(jdate2,sun_eq[0],sun_eq[1],sun_eq[2]);
					jpl.get_moon_position(jdate2,moon_eq[0],moon_eq[1],moon_eq[2]);

					combined_result.precision(16);
					combined_result << setw(16) << jdate2 << " ";

					combined_result.precision(10);
					combined_result << setw(10) << sim_result_drift.dec[idx2] << " "
									<< setw(10) << sim_result_drift.ra[idx2] << " "
									<< setw(14) << sat_eq[0] << " "
									<< setw(14) << sat_eq[1] << " "
									<< setw(14) << sat_eq[2] << " "
									<< setw(14) << sun_eq[0] << " "
									<< setw(14) << sun_eq[1] << " "
									<< setw(14) << sun_eq[2] << " "
									<< setw(14) << moon_eq[0] << " "
									<< setw(14) << moon_eq[1] << " "
									<< setw(14) << moon_eq[2] << " "
									<< setw(5) << 0.0 << " "
									<< setw(5) << 0.0 << " "
									<< setw(10) << sim_result_drift.deepflag[idx2] << " "
									<< setw(5) << sim_result_drift.in_galactic_disk[idx2] << " "
									<< setw(10) << sim_result_drift.exp_time[idx2] << " "
									<< setw(14) << sim_result_drift.tAngle[idx2] << " "
									<< setw(5) << sim_result_drift.in_sun_side[idx2] << " "
									<< setw(10) << sim_result_drift.cmg[idx2] << " "
									<< setw(10) << sim_result_drift.battery[idx2] << " "
									<< setw(10) << sim_result_drift.sun_panel_angle[idx2] << " "
									<< setw(10) << sim_result_drift.saa_time[idx2] << " "
									<< setw(10) << sim_result_drift.sky_id[idx2] 
									<< "\n";
				} else {
					// PrintWarning("failed to get satellite position!");
					cout << "## jdate2 = " << jdate2 << "\n";
					cout << "## cnt2   = " << cnt2 << "\n";
					PrintError("failed to get satellite position!");
				}

				if( idx2 <= idx2_max ){
					idx2++;
					cnt2++;
					cnt_total++;
				}
			}
		}
		
		if( idx1 >= idx1_max && idx2 >= idx2_max ){
			break;
		}
	}

	cout << "\n"
		 << "--> cnt1      = " << cnt1 << "\n"
		 << "--> cnt2      = " << cnt2 << "\n"
		 << "--> cnt_total = " << cnt_total << "\n";

	combined_result.close();
}