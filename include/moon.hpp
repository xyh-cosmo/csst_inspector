#include "container.hpp"
#include "ephemeris_api.hpp"

#ifndef _MOON_HPP_
#define _MOON_HPP_

class Moon{
public:
    Moon();
    ~Moon();
    int check_constraint( Ephemeris& eph, double& jdate, double& ra, double& dec);
};



#endif // _MOON_HPP_