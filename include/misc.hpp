#include <iostream>
#include <string>
#include <sstream>

#ifndef _MISC_HPP_
#define _MISC_HPP_

#if defined(_ENABLE_DEBUG_INFO_)
#define DebugInfo( dbgmsg ) {\
    std::cout << "\n" \
              << "FILE: " << __FILE__ << "\n" \
              << "LINE: " << __LINE__ << "\n" \
              << "DebugInfo: " << dbgmsg << "\n" \
              << std::endl; \
}
#else
#define DebugInfo( info ) { }
#endif

#define PrintWarning( errmsg ) {\
    std::cout << "\n" \
              << "FILE: " << __FILE__ << "\n" \
              << "LINE: " << __LINE__ << "\n" \
              << "Warning: " << errmsg << "\n" \
              << std::endl; \
}

#define PrintError( errmsg ) {\
    std::cout << "\n" \
              << "FILE: " << __FILE__ << "\n" \
              << "LINE: " << __LINE__ << "\n" \
              << "Error: " << errmsg << "\n" \
              << std::endl; \
    exit(0); \
}

#define FuncHeadInfo( msg ) {\
    std::cout << "\n Calling function " \
              << __FUNCTION__ << " : " \
              << msg    \
              << std::endl; \
}

//  ===========================================================================
//  convert int and double into string
//  NOTE: std::to_string() can do the same thing, but low version C++ compilers
//  do not support this!
std::string Int2String(int& i);
std::string Double2String( double& x );

#endif // _MISC_HPP_