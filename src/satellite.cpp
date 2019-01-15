#include "inspector.hpp"

OrbitData::OrbitData(){
    size = 0;
    t = NULL;
    x = NULL;
    y = NULL;
    z = NULL;
#if defined(_INCLUDE_VELOCITY_)
    vx = NULL;
    vy = NULL;
    vz = NULL;
#endif
    t_start = 0;
    t_end = 0;
}

OrbitData::~OrbitData(){
    if( t != NULL ) delete[] t;
    if( x != NULL ) delete[] x;
    if( y != NULL ) delete[] y;
    if( z != NULL ) delete[] z;
#if defined(_INCLUDE_VELOCITY_)
    if( vx != NULL ) delete[] vx;
    if( vy != NULL ) delete[] vy;
    if( vz != NULL ) delete[] vz;
#endif
}

SatelliteOrbit::SatelliteOrbit(){
    orbit_num = 0;
    current_orbit_idx = 0;
    current_t_start = 0;
    current_t_end = 0;
    orbit_data_dir = "";
    lagrange_interp = false;
    status_subastral= false;
}

SatelliteOrbit::SatelliteOrbit(std::string data_dir){
    std::cout << "--> initializing SatlliteOrbit\n";
    orbit_num = 0;
    current_orbit_idx = 0;
    current_t_start = 0;
    current_t_end = 0;
    lagrange_interp = false;

    set_orbit_dir(data_dir);
    read_orbits();
}

SatelliteOrbit::~SatelliteOrbit(){
    for( size_t i=0; i<accel.size(); ++i ){
        gsl_interp_accel_free(accel[i]);
        orbits[i]->~OrbitData();
    }
}


void SatelliteOrbit::use_lagrange_interp(bool opt){
    lagrange_interp = opt;
}

void SatelliteOrbit::set_orbit_dir(std::string data_dir){
    orbit_data_dir = data_dir;
    std::cout << "\n--> orbit_data_dir: " << data_dir << std::endl;
}


int SatelliteOrbit::read_one_orbit(int idx){

    int status = 0;

    std::string orbit_filename = orbit_data_dir + "/" + Int2String(idx) + ".txt";
    std::ifstream orbit_data(orbit_filename.c_str());

    if( !orbit_data.is_open() ){
        return 1;
    }

    std::string line;
    double jtime_tmp;
    double x_tmp, y_tmp, z_tmp;
    std::vector<double> vec_jtime;
    std::vector<double> vec_x, vec_y, vec_z;
#if defined(_INCLUDE_VELOCITY_)
    double vx_tmp, vy_tmp, vz_tmp;
    std::vector<double> vec_vx, vec_vy, vec_vz;
#endif
    while( std::getline(orbit_data,line) ){
        if( line[0] != '#' ){
            std::stringstream stream(line);
#if defined(_INCLUDE_VELOCITY_)
            stream >> jtime_tmp 
                   >> x_tmp >> y_tmp >> z_tmp
                   >> vx_tmp >> vy_tmp >> vz_tmp;
#else
            stream >> jtime_tmp 
                   >> x_tmp >> y_tmp >> z_tmp;
#endif
            vec_jtime.push_back(jtime_tmp);
            vec_x.push_back(x_tmp);
            vec_y.push_back(y_tmp);
            vec_z.push_back(z_tmp);

#if defined(_INCLUDE_VELOCITY_)
            vec_vx.push_back(vx_tmp);
            vec_vy.push_back(vy_tmp);
            vec_vz.push_back(vz_tmp);
#endif
        }
    }

    orbit_data.close();
    
    OrbitData* orbit = new OrbitData;
    
    int add_last_tail = 0;
    int last_idx = idx-1;
    if( idx > 1 ){
        if( std::fabs(vec_jtime[0]-orbits[last_idx-1]->t_end)*86400 < 250 ){
            add_last_tail = 1;
        }
    }

    if( add_last_tail == 1 ){
        // std::cout << " add_ladT_tail = 1\n";
        orbit->size = vec_jtime.size()+1;
        orbit->t = new double[orbit->size];
        orbit->x = new double[orbit->size];
        orbit->y = new double[orbit->size];
        orbit->z = new double[orbit->size];
#if defined(_INCLUDE_VELOCITY_) 
        orbit->vx = new double[orbit->size];
        orbit->vy = new double[orbit->size];
        orbit->vz = new double[orbit->size];
#endif

        orbit->t[0] = orbits[last_idx-1]->t[orbits[last_idx-1]->size-1];
        orbit->x[0] = orbits[last_idx-1]->x[orbits[last_idx-1]->size-1];
        orbit->y[0] = orbits[last_idx-1]->y[orbits[last_idx-1]->size-1];
        orbit->z[0] = orbits[last_idx-1]->z[orbits[last_idx-1]->size-1];
#if defined(_INCLUDE_VELOCITY_)
        orbit->vx[0] = orbits[last_idx-1]->vx[orbits[last_idx-1]->size-1];
        orbit->vy[0] = orbits[last_idx-1]->vy[orbits[last_idx-1]->size-1];
        orbit->vz[0] = orbits[last_idx-1]->vz[orbits[last_idx-1]->size-1];
#endif

        for( int i=1; i<orbit->size; ++i ){
            orbit->t[i] = vec_jtime[i-1];
            orbit->x[i] = vec_x[i-1];
            orbit->y[i] = vec_y[i-1];
            orbit->z[i] = vec_z[i-1];
#if defined(_INCLUDE_VELOCITY_)
            orbit->vx[i] = vec_vx[i-1];
            orbit->vy[i] = vec_vy[i-1];
            orbit->vz[i] = vec_vz[i-1];
#endif
        }    
    }else{
        // std::cout << " add_ladT_tail = 0\n";
        orbit->size = vec_jtime.size();
        orbit->t = new double[orbit->size];
        orbit->x = new double[orbit->size];
        orbit->y = new double[orbit->size];
        orbit->z = new double[orbit->size];
#if defined(_INCLUDE_VELOCITY_)
        orbit->vx = new double[orbit->size];
        orbit->vy = new double[orbit->size];
        orbit->vz = new double[orbit->size];
#endif

        for( int i=0; i<orbit->size; ++i ){
            orbit->t[i] = vec_jtime[i];
            orbit->x[i] = vec_x[i];
            orbit->y[i] = vec_y[i];
            orbit->z[i] = vec_z[i];
#if defined(_INCLUDE_VELOCITY_)
            orbit->vx[i] = vec_vx[i];
            orbit->vy[i] = vec_vy[i];
            orbit->vz[i] = vec_vz[i];
#endif
        }
    }

    orbit->t_start = orbit->t[0];
    orbit->t_end   = orbit->t[orbit->size-1];

    orbits.push_back(orbit);

    gsl_interp_accel *a = gsl_interp_accel_alloc();
    accel.push_back(a);

    return status;
}

void SatelliteOrbit::read_orbits(){
    std::cout << "--> calling " << __FUNCTION__ << " to load orbit data\n";

    if( orbit_data_dir.size() == 0 ){
        PrintError("orbit_data_dir is not set!");
    }

    int status;
    for( int i=1; i<=50; ++i ){
        status = read_one_orbit(i);
        if( status == 0 )
            orbit_num++;
    }

    std::cout << "--> orbit data num = " << orbit_num << "\n";

    t_head = orbits[0]->t_start;
    t_tail = orbits[orbit_num-1]->t_end;
}

void SatelliteOrbit::read_orbits(int num){
    std::cout << "--> calling " << __FUNCTION__ << " to load orbit data\n";

    if( orbit_data_dir.size() == 0 ){
        PrintError("orbit_data_dir is not set!");
    }

    int status;
    for( int i=1; i<=num; ++i ){
        status = read_one_orbit(i);
        if( status == 0 )
            orbit_num++;
    }

    std::cout << "--> orbit data num = " << orbit_num << "\n";

    t_head = orbits[0]->t_start;
    t_tail = orbits[orbit_num-1]->t_end;
}

int SatelliteOrbit::get_satellite_position( double jdate, 
                                            double& xt, 
                                            double& yt, 
                                            double& zt, 
                                            double& dist ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        dist = -999;
        return 1;
    }

    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);

#if defined(_USE_LAGRANGE_INTERP_)
    int idx_start=-1, idx_end=-1;

    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_x=0,tmp_y=0,tmp_z=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_x += orbits[idx]->x[i]*li;
        tmp_y += orbits[idx]->y[i]*li;
        tmp_z += orbits[idx]->z[i]*li;
    }

    xt = tmp_x;
    yt = tmp_y;
    zt = tmp_z;
#else
    double t1,t2;
    double x1,y1,z1,x2,y2,z2;
    
    t1 = orbits[idx]->t[t_idx];
    t2 = orbits[idx]->t[t_idx+1];

    x1 = orbits[idx]->x[t_idx];
    y1 = orbits[idx]->y[t_idx];
    z1 = orbits[idx]->z[t_idx];
    x2 = orbits[idx]->x[t_idx+1];
    y2 = orbits[idx]->y[t_idx+1];
    z2 = orbits[idx]->z[t_idx+1];

    double l1 = sqrt(x1*x1 + y1*y1 + z1*z1);
    double l2 = sqrt(x2*x2 + y2*y2 + z2*z2);
    double theta = acos((x1*x2 + y1*y2 + z1*z2)/(l1*l2));
    double theta1 = (jdate-t1)/(t2-t1)*theta;
    double theta2 = theta-theta1;
    double l = (t2-jdate)/(t2-t1)*l1 + (jdate-t1)/(t2-t1)*l2;

    double sin_theta1 = sin(theta1);
    double sin_theta2 = sin(theta2);

    double x0 = sin_theta2*x1/l1 + sin_theta1*x2/l2;
    double y0 = sin_theta2*y1/l1 + sin_theta1*y2/l2;
    double z0 = sin_theta2*z1/l1 + sin_theta1*z2/l2;
    double l_ = sqrt(x0*x0 + y0*y0 + z0*z0);

    xt = x0*l/l_;
    yt = y0*l/l_;
    zt = z0*l/l_;
#endif

    dist = sqrt( xt*xt + yt*yt + zt*zt );

    return 0;
}

int SatelliteOrbit::get_satellite_position( double jdate, 
                                            double& xt, 
                                            double& yt, 
                                            double& zt ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        return 1;
    }


    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);

#if defined(_USE_LAGRANGE_INTERP_)
    int idx_start=-1, idx_end=-1;

    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_x=0,tmp_y=0,tmp_z=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_x += orbits[idx]->x[i]*li;
        tmp_y += orbits[idx]->y[i]*li;
        tmp_z += orbits[idx]->z[i]*li;
    }

    xt = tmp_x;
    yt = tmp_y;
    zt = tmp_z;
#else
    double t1,t2;
    double x1,y1,z1,x2,y2,z2;

    t1 = orbits[idx]->t[t_idx];
    t2 = orbits[idx]->t[t_idx+1];

    x1 = orbits[idx]->x[t_idx];
    y1 = orbits[idx]->y[t_idx];
    z1 = orbits[idx]->z[t_idx];
    x2 = orbits[idx]->x[t_idx+1];
    y2 = orbits[idx]->y[t_idx+1];
    z2 = orbits[idx]->z[t_idx+1];

    double l1 = sqrt(x1*x1 + y1*y1 + z1*z1);
    double l2 = sqrt(x2*x2 + y2*y2 + z2*z2);
    double theta = acos((x1*x2 + y1*y2 + z1*z2)/(l1*l2));
    double theta1 = (jdate-t1)/(t2-t1)*theta;
    double theta2 = theta-theta1;
    double l = (t2-jdate)/(t2-t1)*l1 + (jdate-t1)/(t2-t1)*l2;

    double sin_theta1 = sin(theta1);
    double sin_theta2 = sin(theta2);

    double x0 = sin_theta2*x1/l1 + sin_theta1*x2/l2;
    double y0 = sin_theta2*y1/l1 + sin_theta1*y2/l2;
    double z0 = sin_theta2*z1/l1 + sin_theta1*z2/l2;
    double l_ = sqrt(x0*x0 + y0*y0 + z0*z0);

    xt = x0*l/l_;
    yt = y0*l/l_;
    zt = z0*l/l_;
#endif

    return 0;
}

int SatelliteOrbit::get_satellite_position( double jdate, double x[], double& dist ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        return 1;
    }


    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);
#if defined(_USE_LAGRANGE_INTERP_)
    int idx_start=-1, idx_end=-1;

    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_x=0,tmp_y=0,tmp_z=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_x += orbits[idx]->x[i]*li;
        tmp_y += orbits[idx]->y[i]*li;
        tmp_z += orbits[idx]->z[i]*li;
    }

    x[0] = tmp_x;
    x[1] = tmp_y;
    x[2] = tmp_z;
#else
    double t1,t2;
    double x1,y1,z1,x2,y2,z2;

    t1 = orbits[idx]->t[t_idx];
    t2 = orbits[idx]->t[t_idx+1];

    x1 = orbits[idx]->x[t_idx];
    y1 = orbits[idx]->y[t_idx];
    z1 = orbits[idx]->z[t_idx];
    x2 = orbits[idx]->x[t_idx+1];
    y2 = orbits[idx]->y[t_idx+1];
    z2 = orbits[idx]->z[t_idx+1];

    double l1 = sqrt(x1*x1 + y1*y1 + z1*z1);
    double l2 = sqrt(x2*x2 + y2*y2 + z2*z2);
    double theta = acos((x1*x2 + y1*y2 + z1*z2)/(l1*l2));
    double theta1 = (jdate-t1)/(t2-t1)*theta;
    double theta2 = theta-theta1;
    double l = (t2-jdate)/(t2-t1)*l1 + (jdate-t1)/(t2-t1)*l2;

    double sin_theta1 = sin(theta1);
    double sin_theta2 = sin(theta2);

    double x0 = sin_theta2*x1/l1 + sin_theta1*x2/l2;
    double y0 = sin_theta2*y1/l1 + sin_theta1*y2/l2;
    double z0 = sin_theta2*z1/l1 + sin_theta1*z2/l2;
    double l_ = sqrt(x0*x0 + y0*y0 + z0*z0);

    x[0] = x0*l/l_;
    x[1] = y0*l/l_;
    x[2] = z0*l/l_;
#endif

    dist = sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);

    return 0;
}

int SatelliteOrbit::get_satellite_position( double jdate, double x[] ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        return 1;
    }


    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);
#if defined(_USE_LAGRANGE_INTERP_)
    int idx_start=-1, idx_end=-1;

    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_x=0,tmp_y=0,tmp_z=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_x += orbits[idx]->x[i]*li;
        tmp_y += orbits[idx]->y[i]*li;
        tmp_z += orbits[idx]->z[i]*li;
    }

    x[0] = tmp_x;
    x[1] = tmp_y;
    x[2] = tmp_z;
#else
    double t1,t2;
    double x1,y1,z1,x2,y2,z2;

    t1 = orbits[idx]->t[t_idx];
    t2 = orbits[idx]->t[t_idx+1];

    x1 = orbits[idx]->x[t_idx];
    y1 = orbits[idx]->y[t_idx];
    z1 = orbits[idx]->z[t_idx];
    x2 = orbits[idx]->x[t_idx+1];
    y2 = orbits[idx]->y[t_idx+1];
    z2 = orbits[idx]->z[t_idx+1];

    double l1 = sqrt(x1*x1 + y1*y1 + z1*z1);
    double l2 = sqrt(x2*x2 + y2*y2 + z2*z2);
    double theta = acos((x1*x2 + y1*y2 + z1*z2)/(l1*l2));
    double theta1 = (jdate-t1)/(t2-t1)*theta;
    double theta2 = theta-theta1;
    double l = (t2-jdate)/(t2-t1)*l1 + (jdate-t1)/(t2-t1)*l2;

    double sin_theta1 = sin(theta1);
    double sin_theta2 = sin(theta2);

    double x0 = sin_theta2*x1/l1 + sin_theta1*x2/l2;
    double y0 = sin_theta2*y1/l1 + sin_theta1*y2/l2;
    double z0 = sin_theta2*z1/l1 + sin_theta1*z2/l2;
    double l_ = sqrt(x0*x0 + y0*y0 + z0*z0);

    x[0] = x0*l/l_;
    x[1] = y0*l/l_;
    x[2] = z0*l/l_;
#endif

    return 0;
}


int SatelliteOrbit::get_satellite_velocity( double jdate, 
                                            double& vx, 
                                            double& vy, 
                                            double& vz ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        return 1;
    }

    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);

    int idx_start=-1, idx_end=-1;
    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_vx=0,tmp_vy=0,tmp_vz=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_vx += orbits[idx]->vx[i]*li;
        tmp_vy += orbits[idx]->vy[i]*li;
        tmp_vz += orbits[idx]->vz[i]*li;
    }

    vx = tmp_vx;
    vy = tmp_vy;
    vz = tmp_vz;

    return 0;
}

int SatelliteOrbit::get_satellite_velocity( double jdate, double v[] ){
    int idx=-1;

    for( int i=0; i<orbit_num; ++i ){
        if( jdate >= orbits[i]->t_start && jdate <= (orbits[i]->t_end-1e-15) ){
            idx = i;
            break;
        }
    }

    if( idx == -1 ){
        return 1;
    }


    int t_idx = gsl_interp_accel_find(  accel[idx],
                                        orbits[idx]->t,
                                        orbits[idx]->size,
                                        jdate);
    int idx_start=-1, idx_end=-1;

    if( t_idx < 3 ){
        idx_start = 0;
        idx_end = idx_start + 8;
    } else if( t_idx > orbits[idx]->size-5 ){
        idx_end = orbits[idx]->size-1;
        idx_start = idx_end-8;
    } else {
        idx_start = t_idx-3;
        idx_end = idx_start+8;
    }

    double tmp_vx=0,tmp_vy=0,tmp_vz=0;
    for( int i=idx_start; i<=idx_end; ++i ){
        double li=1.0;
        for( int j=idx_start; j<=idx_end; ++j ){
            if( j!= i){
                li *= (jdate-orbits[idx]->t[j])/(orbits[idx]->t[i]-orbits[idx]->t[j]);
            }
        }
        tmp_vx += orbits[idx]->vx[i]*li;
        tmp_vy += orbits[idx]->vy[i]*li;
        tmp_vz += orbits[idx]->vz[i]*li;
    }

    v[0] = tmp_vx;
    v[1] = tmp_vy;
    v[2] = tmp_vz;

    return 0;
}

void SatelliteOrbit::init_subastral( Ephemeris& jpl405, double jd0 ){
    double x,y,z,ra,dec;
    double t0 = floor(jd0);
    if( jpl405.get_sun_position(t0,x,y,z) == 0 ){
        double x_sun[3] = {x,y,z};
        Cartesian2Spherical_ra_dec_degree(x_sun, ra, dec);
        ra0_greenwich = ra;
        jdate0 = t0;
    } else {
        PrintError("failed to get Sun's position in Julian date: " + Double2String(t0)); 
    }
}

void SatelliteOrbit::get_subastral_position(double jdate, double& ra_ref, double& dec_ref){
    double x_sat[3], ra, dec;
    if( get_satellite_position(jdate,x_sat) == 0 ){
        Cartesian2Spherical_ra_dec_degree(x_sat,ra,dec);
    } else {
        PrintError("failed to get satellite position!");
    }

    double delta_jt = jdate - jdate0;
    double d_ra = fmod(delta_jt*2*M_PI,2*M_PI)*180/M_PI;
    double ra_greenwich = ra0_greenwich + d_ra;

    double x0 = cos(ra_greenwich*M_PI/180);
    double y0 = sin(ra_greenwich*M_PI/180);
    double x1 = cos(ra*M_PI/180);
    double y1 = sin(ra*M_PI/180);
    double cosval = x0*x1+y0*y1;
    double angle = acos(cosval)*180/M_PI;

    double cross = x1*y0-x0*y1;
    angle = angle*(-cross/fabs(cross));

    ra_ref  = angle;
    dec_ref = dec;
}
