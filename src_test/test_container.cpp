#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

    if( argc != 2 ){
        cout << "usage: " << argv[0] << " simulation_result.txt\n";
        exit(0);
    }

    string result_file(argv[1]);
    SimulationResults SR(result_file,1000);
    SR.print_summary();
}
