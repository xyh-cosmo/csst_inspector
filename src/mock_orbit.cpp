#include "inspector.hpp"

MockOrbit::MockOrbit(){
    e = 0.95;
    a = 1.0;
    c = a*e;
    b = sqrt(a*a-c*c);
    k = 1.0;
    
    orbit_num = 1.0;
    T = 92*60; // minitues
    dt= 60; // seconds
    tmax = T; // seconds

    ode = ODE_orbit;
}

MockOrbit::~MockOrbit(){
// do nothing
}

void MockOrbit::set_eccentricity(double e){
    this->e = e;
}

void MockOrbit::set_period(double T){
    this->T = T*60; // input T has unit of minitues

}

void MockOrbit::set_orbit_num(double num){
    this->orbit_num = num;
    this->tmax = this->orbit_num*this->T;
}

void MockOrbit::set_time_interval(double dt){
    this->dt = dt;
}


void MockOrbit::get_initial_condition( double& x, double& y, double& vx, double& vy){
    a = pow(0.5*T/M_PI,2./3.);
    c = a*e;
    b = sqrt(a*a-c*c);
    x = -(a-c);
    y = 0;

    double E = -0.5/a; // total energy is negative
    double r = sqrt(x*x+y*y);
    vx= 0;
    vy= -1.0*sqrt(2*E + 2.0/r);
}

void MockOrbit::write_orbit(std::string filename){

    gsl_odeiv2_system sys = {ode,NULL,4,NULL};
    gsl_odeiv2_driver *driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk8pd,1e-5,1e-10,1e-10);

    double t=0, ti;
    double y[4]; // = {x,y,vx,vy}
    get_initial_condition(y[0],y[1],y[2],y[3]);

    std::ofstream ofile(filename.c_str());

    if( !ofile.is_open() ){
        std::cout << "==> failed to open file: "
                  << filename + " for saving orbit data"
                  << "\n==> please check the output file's path\n";
    }

    ofile << std::left << std::setw(15) << "#time "
          << std::left << std::setw(15) << " X "
          << std::left << std::setw(15) << " Y "
          << std::left << std::setw(15) << " Z "
          << std::left << std::setw(15) << " VX "
          << std::left << std::setw(15) << " VY "
          << std::left << std::setw(15) << " VZ "
          << "\n";

    while( t <= tmax ){
        ti = t + dt;
        int status = gsl_odeiv2_driver_apply(driver,
                                             &t,
                                             ti,
                                             y);

        if( status != GSL_SUCCESS ){
            std::cout << "error, return value = " << status << "\n";
        }

        std::cout.precision(12);

        ofile << std::left << std::setw(15) << t
              << std::left << std::setw(15) << y[0]
              << std::left << std::setw(15) << y[1]
              << std::left << std::setw(15) << 0
              << std::left << std::setw(15) << y[2]
              << std::left << std::setw(15) << y[3]
              << std::left << std::setw(15) << 0
              << "\n";
    }

    gsl_odeiv2_driver_free(driver);

    ofile.close();
}

int ODE_orbit( double t, const double f[], double dfdt[], void* params){
    double x  = f[0];
    double y  = f[1];
    double vx = f[2];
    double vy = f[3];
    double r2 = x*x+y*y;
    double r3 = pow(r2,1.5);

    dfdt[0] = vx;
    dfdt[1] = vy;
    dfdt[2] = -x/r3;
    dfdt[3] = -y/r3;

    return GSL_SUCCESS;
}