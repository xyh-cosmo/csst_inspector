//  测试利用拉格朗日插值方法获取卫星位置和速度的精度

#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv [] ){
    if( argc != 3 ){
        cout << "usage: " << argv[0] << " orbite_data_dir orbit_ref.txt\n";
        exit(0);
    }

    string dir(argv[1]);
    SatelliteOrbit sat_orbit(dir);

    string ref(argv[2]);
    std::ifstream orbit_data(ref.c_str());
    std::string line;
    double t_tmp;
    double x_tmp, y_tmp, z_tmp;
    double vx_tmp, vy_tmp, vz_tmp;
    std::vector<double> vec_t;
    std::vector<double> vec_x, vec_y, vec_z;
    std::vector<double> vec_vx, vec_vy, vec_vz;
    
    while( std::getline(orbit_data,line) ){
        if( line[0] != '#' ){
            std::stringstream stream(line);
            stream >> t_tmp 
                   >> x_tmp >> y_tmp >> z_tmp
                   >> vx_tmp >> vy_tmp >> vz_tmp;
            vec_t.push_back(t_tmp);
            vec_x.push_back(x_tmp);
            vec_y.push_back(y_tmp);
            vec_z.push_back(z_tmp);
            vec_vx.push_back(vx_tmp);
            vec_vy.push_back(vy_tmp);
            vec_vz.push_back(vz_tmp);
        }
    }

    // uncomment the following to use Lagrange interpolation method
    // sat_orbit.use_lagrange_interp(true);

    string ofile_name = "xxx.txt";
    ofstream ofile(ofile_name.c_str());

    double dist,x,y,z;
    for( size_t i=0; i<vec_t.size(); ++i ){
        if( sat_orbit.get_satellite_position(vec_t[i],x,y,z,dist) == 0 ){
            double r = sqrt(vec_x[i]*vec_x[i]+vec_y[i]*vec_y[i]);
            // double dx = fabs((vec_x[i] - x)/vec_x[i]);
            // double dy = fabs((vec_y[i] - y)/vec_y[i]);
            double dx = fabs((vec_x[i] - x)/r);
            double dy = fabs((vec_y[i] - y)/r);

            double vx,vy,vz;
            sat_orbit.get_satellite_velocity(vec_t[i],vx,vy,vz);
            double v = sqrt(vx*vx + vy*vy + vz*vz);
            double dvx = fabs((vec_vx[i] - vx)/v);
            double dvy = fabs((vec_vy[i] - vy)/v);

            cout.precision(12);
            ofile <<  scientific << setw(15) << dx
                  << " " << scientific << setw(15) << dy
                  << " " << scientific << setw(15) << dvx
                  << " " << scientific << setw(15) << dvy
                  << "\n";
        }
    }
    
    ofile.close();

    return 0;
}
