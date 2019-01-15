#include "misc.hpp"

std::string Int2String(int& i){
    std::stringstream stream;
    stream << i;
    return stream.str();
}

std::string Double2String( double& x){
    std::stringstream stream;
    stream << x;
    return stream.str();
}
