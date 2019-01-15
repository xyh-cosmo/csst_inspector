#include "GSL_func.hpp"

#ifndef _ROTATION_HPP_
#define _ROTATION_HPP_

//  given a rotation angle, returns the time needed to rotate that angle.
class Rotation_Time{
private:
    GSL_Interpolator_1d interp1d;
public:
//  methods
    Rotation_Time();
    ~Rotation_Time();
    double get_time(double t);
    // void init();
};

#endif