#include "inspector.hpp"

int Inspector::test_cmg( bool use_5y, bool moving_window ){
    std::cout << "--> calling " << __FUNCTION__ << " to check CMG constraint\n";

    cmg_list.use_5y = use_5y;

    cmg_list.moving_window = moving_window;
    cmg_list.check( sim_results, sim_results.size );

    return 0;
}