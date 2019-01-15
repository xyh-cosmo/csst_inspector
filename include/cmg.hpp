#include <iostream>
#include <deque>

#include "GSL_func.hpp"
#include "container.hpp"
#include "rotation.hpp"
#include "misc.hpp"

#ifndef _CMG_HPP_
#define _CMG_HPP_

class CMG_use{
private:
    double alpha;   // heat production rate, set to default value 1.0
    double beta;    // heat radiation rate, set to default value 1/6 (min-value)
    GSL_Interpolator_1d interp1d;
    double func_cmg_use(double angle);
public:
    double get_use( double angle, bool use_5y=false );
    double get_use_5y( double angle );
    void set_alpha(double alpha_new);
    void set_beta(double beta_new);
    CMG_use();
    ~CMG_use();
};

class CMG_Node{
public:
    double obs_time;    // Julian date in unit of days
    double exp_time;    //  exposure time in unit of seconds
    double rotation_angle;  // angle rotated before current observation getting started
    double rotation_time;   // time needed to make the rotation
    double time_head, time_tail;
    CMG_Node();
    CMG_Node(double otime, double exptime, double angle, double rtime);
    ~CMG_Node();
};


class CMG_List{
public:
    bool moving_window;
    bool use_5y;
    double one_orbit_time;
    double cmg_total_use;
    double cmg_total_time;
    double time_head, time_tail;

    double Q_gain, Q_lost;

    std::deque<CMG_Node> list;
    CMG_use cmg_use;
    Rotation_Time rotation_time;

    void use_moving_window( bool opt=true );
    int add_a_node( CMG_Node node );
    int add_a_node2( CMG_Node node );   // use a simplified CMG model to calculate CMG temerature state
    void moving_window_check( SimulationResults& sr, int first_row, int last_row );
    void one_orbit_check( SimulationResults& sr, int first_row, int last_row );
    void check( SimulationResults& sr, int first_n=100 );

    void reset();
    CMG_List();
    ~CMG_List();
};


#endif // _CMG_HPP_