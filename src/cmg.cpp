#include "inspector.hpp"

////////////////////////////////////////////////////////
//  CMG_use

CMG_use::CMG_use(){
    alpha = 1.0;
    beta = 1.0/6.0;
    double angle[50], use[50];
    for( int i=0; i<50; ++i ){
        angle[i] = i*180./(49);
        use[i] = func_cmg_use(angle[i]);
    }
    interp1d.init(angle,use,50,0);
}

CMG_use::~CMG_use(){
    // ~interp1d();
    // interp1d.~GSL_Interpolator_1d();
}

void CMG_use::set_alpha(double alpha_new){
    alpha = alpha_new;
}

void CMG_use::set_beta(double beta_new){
    beta = beta_new;
}

double CMG_use::func_cmg_use(double tAngle){
    double cmg_use=0.0;
    double tm_k;
    if( tAngle <= 5.0 ) {
        cmg_use = 0;
    } else if( tAngle <= 10.0 ) {
        tm_k = (1/29.0-0)/(10.0-5.0);
        cmg_use = tm_k*(tAngle-5);
    } else if( tAngle <= 20.0 ) {
        tm_k = (1/19.0-1/29.0)/(20.0-10.0);
        cmg_use = tm_k*(tAngle-10)+1/29.0;
    } else if( tAngle <= 35.0 ) {
        tm_k = (1/13.0-1/19.0)/(35.0-20.0);
        cmg_use = tm_k*(tAngle-20)+1/19.0;
    } else if( tAngle <= 45.0 ) {
        tm_k = (1/10.0-1/13.0)/(45.0-35.0);
        cmg_use = tm_k*(tAngle-35)+1/13.0;
    } else if( tAngle <= 75.0 ) {
        tm_k = (1/6.0-1/10.0)/(75.0-45.0);
        cmg_use = tm_k*(tAngle-45)+1/10.0;
    } else if( tAngle <= 90.0 ) {
        tm_k = (1/5.0-1/6.0)/(90.0-75.0);
        cmg_use = tm_k*(tAngle-75)+1/6.0;
    } else if( tAngle <= 135.0 ) {
        tm_k = (1/3.0-1/5.0)/(135.0-90.0);
        cmg_use = tm_k*(tAngle-90)+1/5.0;
    } else if( tAngle <= 180.0 ) {
        tm_k = (1/2.0-1/3.0)/(180.0-135.0);
        cmg_use = tm_k*(tAngle-135)+1/3.0;
    }

    return cmg_use;
}

double CMG_use::get_use( double angle, bool use_5y ){
    if( use_5y == true ){
        // std::cout << " use_5y is TRUE!\n";
        return get_use_5y(angle);
    }
    else{
        // std::cout << " use_5y is FALSE!\n";
        return interp1d.get_value(angle);
    }

    // exit(0);
}

double CMG_use::get_use_5y( double angle ){

    // std::cout << "-----> calling get_use_5y \n";

    double cmg_use = 0;

    if( angle < 5.0 )
        cmg_use = 0;
    else if( angle <= 10.0 )
        cmg_use = 1./29.;
    else if( angle <= 20.0 )
        cmg_use = 1./19.;
    else if( angle <= 35.0 )
        cmg_use = 1./13.;
    else if( angle <= 45.0 )
        cmg_use = 1./10.;
    else if( angle <= 75.0 )
        cmg_use = 1./6.;
    else if( angle <= 90.0 )
        cmg_use = 1./5.0;
    else if( angle <= 135.0 )
        cmg_use = 1./3.;
    else if( angle <= 180.0 )
        cmg_use = 1./2.;

    return cmg_use;
}

////////////////////////////////////////////////////////
//  CMG_Node
CMG_Node::CMG_Node(){
    obs_time = 0;
    exp_time = 0;
    rotation_angle = 0;
    rotation_time = 0;
    time_head = 0;
    time_tail = 0;
}

CMG_Node::CMG_Node(double otime, double exptime, double angle, double rtime){
    obs_time = otime;
    exp_time = exptime;
    rotation_angle = angle;
    rotation_time = rtime;
    time_head = obs_time - rotation_time/86400;
    time_tail = obs_time + exp_time/86400;
}

CMG_Node::~CMG_Node(){
//  do nothing
}


////////////////////////////////////////////////////////
//  CMG_List

CMG_List::CMG_List(){
    moving_window = false;
    use_5y = false;
    one_orbit_time = 0.0638;   // unit == day
    // one_orbit_time = 0.063;   // unit == day
    cmg_total_use = 0;
    cmg_total_time= 0;
    time_head = 0;
    time_tail = 0;
}

CMG_List::~CMG_List(){
    //  do nothing
}

void CMG_List::use_moving_window( bool opt ){
    moving_window = opt;
}

void CMG_List::reset(){
    while( list.size() > 0 ){
        list.pop_back();
    }
    cmg_total_time = 0;
    cmg_total_use  = 0;
    time_head = 0;
    time_tail = 0;
}

int CMG_List::add_a_node( CMG_Node node ){

    int flag = 0;
    int list_size = list.size();

    if( list_size > 0 ){

        double time_increased = (node.time_head-list[list_size-1].time_tail);
        
        if( time_increased > one_orbit_time ){
        //  if an new node is added after a peorid longer than one_orbit_time, 
        //  then CMG usage can be though as been re-set to 0
            time_head = node.time_head;
            time_tail = node.time_tail;
            cmg_total_use = cmg_use.get_use(node.rotation_angle, use_5y);
            cmg_total_time= time_tail - time_head;
            return 0;
        } else if( time_increased > 0.5*one_orbit_time ){
            if( cmg_total_use > 0.5 )
                PrintWarning("cmg_total_use is larger than 0.5 after 1/2 orbit time!");
        }

        if( (time_tail - node.time_head)*86400 > 1e-3 ){
            flag = 1;
            PrintWarning("add_a_noe::flag == 1!");
        }

        list.push_back(node);
        double total_time_tmp = cmg_total_time + (node.time_tail-list[list_size-1].time_tail);
        double total_use_tmp = cmg_total_use + cmg_use.get_use(node.rotation_angle, use_5y);
        
        while( total_time_tmp > one_orbit_time ){
            total_time_tmp -= (list[1].time_head - list[0].time_head);
            total_use_tmp -= cmg_use.get_use(list[0].rotation_angle, use_5y);

            if( list.size() >= 2 ) // adding this test is for security!
                list.pop_front();
        }

        // list.push_back(node);
        cmg_total_time = total_time_tmp;
        cmg_total_use = total_use_tmp;
        time_head = list[0].time_head;
        time_tail = list[list.size()-1].time_tail;

        // if( cmg_total_use > 1.0 ){
        //     PrintWarning("cmg_total_use = " + Double2String(cmg_total_use));
        //     std::cout << "node.rotation_angle = " << node.rotation_angle << "\n";
        //     std::cout << "delta_cmg_use       = " << cmg_use.get_use(node.rotation_angle, use_5y) << "\n"; 
        // }

        double time_diff = time_tail - time_head;
        if( std::fabs(std::fabs(time_diff)-cmg_total_time) > 1e-3 ){
            flag = 2;
        }
    } else if ( list_size == 0 ) {
        list.push_back(node);
        cmg_total_use = cmg_use.get_use(node.rotation_angle, use_5y);
        cmg_total_time= node.time_tail - node.time_head;
        time_head = node.time_head;
        time_tail = node.time_tail;
    }

    return flag;
}


void CMG_List::moving_window_check( SimulationResults& sr, int first_row, int last_row ){

    if( sr.size == 0 )
        PrintError("size of simulation result is 0, stop here!");

    if( first_row < 0 )
        PrintError("first_row should be non-negative!");

    if( last_row > sr.size )
        PrintError("last_row should not be larger than sr.size!");

    std::cout << "\n=> start running CMG constraint check ...\n";
    std::cout << "==> using moving window check option\n";
    std::cout << "-> First row: " << first_row << ", Last row: " << last_row << "\n";

    int cnt_good = 0;
    int cnt_warn1=0, cnt_warn2=0;
    
    int cnt_cmg_good=0,cnt_cmg_bad=0;
    int cnt_cmg_larger_than_1=0;
    int cnt_cmg_larger_than_1_005=0;
    int cnt_cmg_larger_than_1_01=0;
    int cnt_cmg_larger_than_1_02=0;
    int cnt_cmg_larger_than_1_03=0;
    int cnt_cmg_larger_than_1_04=0;
    int cnt_cmg_larger_than_1_05=0;

    double cmg_use_max = 0;

    int i = first_row;
    while( i < last_row ){
        
        double otime = sr.jdate[i];
        double exp_time = sr.exp_time[i];
        double tAngle = sr.tAngle[i];
        double rtime = rotation_time.get_time(tAngle);
        CMG_Node node(otime, exp_time, tAngle, rtime);

        int flag = add_a_node(node);
        switch (flag){
            case 0:
                cnt_good++;
                break;
            case 1:
                cnt_warn1++;
                break;
            case 2:
                cnt_warn2++;
                break;
            default:
                PrintError("Unkonw value for flag");
                break;
        }

        if( cmg_use_max < cmg_total_use )
            cmg_use_max = cmg_total_use;

        if( cmg_total_use > 1.0 ){
            // std::cout << " i = " << i << "\n";
            cnt_cmg_larger_than_1++;
        }

        if( cmg_total_use > 1.005 )
            cnt_cmg_larger_than_1_005++;

        if( cmg_total_use > 1.01 )
            cnt_cmg_larger_than_1_01++;

        if( cmg_total_use > 1.02 )
            cnt_cmg_larger_than_1_02++;

        if( cmg_total_use > 1.03 )
            cnt_cmg_larger_than_1_03++;

        if( cmg_total_use > 1.04 )
            cnt_cmg_larger_than_1_04++;

        if( cmg_total_use > 1.05 )
            cnt_cmg_larger_than_1_05++;

        if( cmg_use_max > 1.0 )
            cnt_cmg_bad++;
        else
            cnt_cmg_good++;
        i++;
    }

    std::cout << "-> CMG constraint has been checked, results are the following:\n"
              << "-> cnt_good  = " << cnt_good << "\n"
              << "-> cnt_warn1 = " << cnt_warn1 << "\n"
              << "-> cnt_warn2 = " << cnt_warn2 << "\n"
              << "-> CMG max value: " << cmg_use_max << "\n"
              << "-> # of CMG_value > 1 is: " << cnt_cmg_larger_than_1 << "\n"
              << "-> # of CMG_value > 1.005 is: " << cnt_cmg_larger_than_1_005 << "\n"
              << "-> # of CMG_value > 1.01 is: " << cnt_cmg_larger_than_1_01 << "\n"
              << "-> # of CMG_value > 1.02 is: " << cnt_cmg_larger_than_1_02 << "\n"
              << "-> # of CMG_value > 1.03 is: " << cnt_cmg_larger_than_1_03 << "\n"
              << "-> # of CMG_value > 1.04 is: " << cnt_cmg_larger_than_1_04 << "\n"
              << "-> # of CMG_value > 1.05 is: " << cnt_cmg_larger_than_1_05 << "\n"
              << "\n";

}

//  this is cloned from "CMG_List::moving_window_check"
void CMG_List::one_orbit_check( SimulationResults& sr, int first_row, int last_row ){

    if( sr.size == 0 )
        PrintError("size of simulation result is 0, stop here!");

    if( first_row < 0 )
        PrintError("first_row should be non-negative!");

    if( last_row > sr.size )
        PrintError("last_row should not be larger than sr.size!");

    std::cout << "\n=> start running CMG constraint check ...\n";
    std::cout << "==> using one orbit check option\n";
    std::cout << "-> First row: " << first_row << ", Last row: " << last_row << "\n";

    int cnt_good = 0;
    int cnt_warn1=0, cnt_warn2=0;
    
    int cnt_cmg_good=0,cnt_cmg_bad=0;
    int cnt_cmg_larger_than_1=0;
    int cnt_cmg_larger_than_1_005=0;
    int cnt_cmg_larger_than_1_01=0;
    int cnt_cmg_larger_than_1_02=0;
    int cnt_cmg_larger_than_1_03=0;
    int cnt_cmg_larger_than_1_04=0;
    int cnt_cmg_larger_than_1_05=0;

    double cmg_use_max = 0;

    int i = first_row;
    while( i < last_row ){
        
        double otime = sr.jdate[i];
        double exp_time = sr.exp_time[i];
        double tAngle = sr.tAngle[i];
        double rtime = rotation_time.get_time(tAngle);
        CMG_Node node(otime, exp_time, tAngle, rtime);

        int flag = add_a_node(node);
        switch (flag){
            case 0:
                cnt_good++;
                break;
            case 1:
                cnt_warn1++;
                break;
            case 2:
                cnt_warn2++;
                break;
            default:
                PrintError("Unkonw value for flag");
                break;
        }

        if( cmg_use_max < cmg_total_use )
            cmg_use_max = cmg_total_use;

        if( cmg_total_use > 1.0 )
            cnt_cmg_larger_than_1++;

        if( cmg_total_use > 1.005 )
            cnt_cmg_larger_than_1_005++;

        if( cmg_total_use > 1.01 )
            cnt_cmg_larger_than_1_01++;

        if( cmg_total_use > 1.02 )
            cnt_cmg_larger_than_1_02++;

        if( cmg_total_use > 1.03 )
            cnt_cmg_larger_than_1_03++;

        if( cmg_total_use > 1.04 )
            cnt_cmg_larger_than_1_04++;

        if( cmg_total_use > 1.05 )
            cnt_cmg_larger_than_1_05++;

        if( cmg_use_max > 1.0 )
            cnt_cmg_bad++;
        else
            cnt_cmg_good++;
        i++;
    }

    std::cout << "-> CMG constraint has been checked, results are the following:\n"
              << "-> cnt_good  = " << cnt_good << "\n"
              << "-> cnt_warn1 = " << cnt_warn1 << "\n"
              << "-> cnt_warn2 = " << cnt_warn2 << "\n"
              << "-> CMG max value: " << cmg_use_max << "\n"
              << "-> # of CMG_value > 1 is: " << cnt_cmg_larger_than_1 << "\n"
              << "-> # of CMG_value > 1.005 is: " << cnt_cmg_larger_than_1_005 << "\n"
              << "-> # of CMG_value > 1.01 is: " << cnt_cmg_larger_than_1_01 << "\n"
              << "-> # of CMG_value > 1.02 is: " << cnt_cmg_larger_than_1_02 << "\n"
              << "-> # of CMG_value > 1.03 is: " << cnt_cmg_larger_than_1_03 << "\n"
              << "-> # of CMG_value > 1.04 is: " << cnt_cmg_larger_than_1_04 << "\n"
              << "-> # of CMG_value > 1.05 is: " << cnt_cmg_larger_than_1_05 << "\n"
              << "\n";

}

void CMG_List::check( SimulationResults& sr, int first_n ){
    if( moving_window ){
        std::cout << "--> calling moving_window_check(**)\n";
        moving_window_check(sr,0,first_n);
    }
    else{
        std::cout << "--> calling one_orbit_check(**)\n";
        one_orbit_check(sr,0,first_n);
    }
}