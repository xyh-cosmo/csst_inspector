// #include "container.hpp"
// #include "ephemeris_api.hpp"

#ifndef _SAA_HPP_
#define _SAA_HPP_

//	given a subastral position of the satellite/telescope, check
//	whether it is inside the SAA zone.
class SAA_Condition{
private:
    double rangePoint[5][2];
    double area_min;
    double area_tolerance;
public:
	SAA_Condition();

    double compute_area_min();
    void set_area_tolerance(double tol);
    
    int test_saa1(double& ra, double& dec);
    int test_saa2(double& ra, double& dec);

    int check(double& ra_ref, double& dec_ref);
};


#endif // _SAA_HPP_
