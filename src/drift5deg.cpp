#include "drift5deg.hpp"


SubastralPosition::SubastralPosition(){
	earthStartTime = -1;
	initialGreenWithRa = -1;
}


SubastralPosition::~SubastralPosition(){
//	do nothing
}

void SubastralPosition::set_greenwich_ra(){
	

void SubastralPosition::set_start_time( double& t_start ){
	earthStartTime = t_start;
	set_greenwich_ra();
}
