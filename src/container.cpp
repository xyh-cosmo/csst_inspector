#include "inspector.hpp"


SimulationResults::SimulationResults(){
    size = 0;
}

SimulationResults::SimulationResults(std::string& result_file, int num){
    load_results(result_file,num);
}

SimulationResults::~SimulationResults(){
// do nothing
}

void SimulationResults::load_results(std::string& result_file, int num){

    std::cout << "\n=> Loading simulation results from\n" << result_file << std::endl;

    std::ifstream infile(result_file.c_str());
    std::string line;

    int in_galactic_disk_tmp;
    int in_sun_side_tmp;
    int sky_id_tmp;
    double jdate_tmp;
    double ra_tmp,dec_tmp;
    double sat_x_eq_tmp,sat_y_eq_tmp,sat_z_eq_tmp;
    double sun_x_eq_tmp,sun_y_eq_tmp,sun_z_eq_tmp;
    double moon_x_eq_tmp,moon_y_eq_tmp,moon_z_eq_tmp;
    double area_norm_tmp, area_deep_tmp;
    double deepflag_tmp;
    double exp_time_tmp;
    double tAngle_tmp;
    double cmg_tmp;
    double battery_tmp;
    double sun_panel_angle_tmp;
    double saa_time_tmp;

    int size_max = 99999999;
    if( num > 0 ) size_max = num;

    size = 0;
    while( std::getline(infile,line) && size < size_max ){
        if( line[0] != '#' ){
            size++;
            std::stringstream stream(line);
            stream >> jdate_tmp 
                >> dec_tmp >> ra_tmp 
                >> sat_x_eq_tmp >> sat_y_eq_tmp >> sat_z_eq_tmp 
                >> sun_x_eq_tmp >> sun_y_eq_tmp >> sun_z_eq_tmp
                >> moon_x_eq_tmp >> moon_y_eq_tmp >> moon_z_eq_tmp
                >> area_norm_tmp >> area_deep_tmp
                >> deepflag_tmp 
                >> in_galactic_disk_tmp
                >> exp_time_tmp
                >> tAngle_tmp
                >> in_sun_side_tmp
                >> cmg_tmp
                >> battery_tmp
                >> sun_panel_angle_tmp
                >> saa_time_tmp
                >> sky_id_tmp;

            jdate.push_back(jdate_tmp);
            ra.push_back(ra_tmp);
            dec.push_back(dec_tmp);
            sat_x_eq.push_back(sat_x_eq_tmp);
            sat_y_eq.push_back(sat_y_eq_tmp);
            sat_z_eq.push_back(sat_z_eq_tmp);
            sun_x_eq.push_back(sun_x_eq_tmp);
            sun_y_eq.push_back(sun_y_eq_tmp);
            sun_z_eq.push_back(sun_z_eq_tmp);
            moon_x_eq.push_back(moon_x_eq_tmp);
            moon_y_eq.push_back(moon_y_eq_tmp);
            moon_z_eq.push_back(moon_z_eq_tmp);
            deepflag.push_back(deepflag_tmp);
            in_galactic_disk.push_back(in_galactic_disk_tmp);
            exp_time.push_back(exp_time_tmp);
            tAngle.push_back(tAngle_tmp);
            in_sun_side.push_back(in_sun_side_tmp);
            cmg.push_back(cmg_tmp);
            battery.push_back(battery_tmp);
            sun_panel_angle.push_back(sun_panel_angle_tmp);
            saa_time.push_back(saa_time_tmp);
            sky_id.push_back(sky_id_tmp);
        }
    }

    infile.close();

    print_summary(-1);
}

void SimulationResults::print_summary(int num){
    if( num == -1 ){
        std::cout << "\n=> summary of simulation results:" << std::endl;
        std::cout << "=> total number of exposures: " << size << std::endl;
    } else if( num > 0 ){
        for( int i=0; i<num; ++i ){
            std::cout << " tAngle[" << i << "] = " << tAngle[i]
                      << " cmg[" << i << "] = " << cmg[i] << std::endl;
        }
    }
}

void SimulationResults::exposure_info(int i){
    std::cout << "\n=> information of " << i << "-th exposure:\n";
    std::cout << " exposure starts at Julian day: " << std::setprecision(15) << jdate[i] << "\n"
              << " ra : " << ra[i] << "\n"
              << " dec: " << dec[i] << "\n"
              << " rot_angle: " << tAngle[i] << "\n"
              << " exp_time: " << exp_time[i] << "\n";
}