#include "inspector.hpp"


//  Rotation_Time
Rotation_Time::Rotation_Time(){
    double angle[4] = {1,20,45,180};
    double time[4] = {80,127,196,581};
    interp1d.init(angle,time,4,0);
}

Rotation_Time::~Rotation_Time(){
    // interp1d.~interp1d();
}

double Rotation_Time::get_time(double angle){

    if( angle < 0 ){
        PrintError(" angle is less than 0.0! ");
    }
    else if( angle > 180 ){
        PrintError(" angle is greater than 180.0! ");
    }

    if( angle <= 1.0 )
        return 70.0;
    else
        return interp1d.get_value(angle);
}
