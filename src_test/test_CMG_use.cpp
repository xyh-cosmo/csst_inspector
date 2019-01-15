#include "inspector.hpp"

int main( int argc, char* argv[] ){
    CMG_use cmg_use;
    std::cout.precision(10);
    for( double ang=0; ang<=180; ang+=0.5 ){
        std::cout << std::setw(10) << ang << std::setw(18) << cmg_use.get_use(ang) << std::endl;
    }
}
