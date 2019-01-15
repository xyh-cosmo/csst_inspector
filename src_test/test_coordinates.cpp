#include "inspector.hpp"

using namespace std;


int main( int argc, char* argv[] ){
	if( argc != 3){
		cout << " usage:\n" << argv[0]
			 << " ra_in dec_in (in degrees)\n";
		exit(0);
	}

	double ra_in = atof(argv[1]);
	double dec_in= atof(argv[2]);

 
	double x[3];
	x[0] = cos(dec_in*M_PI/180)*cos(ra_in*M_PI/180);
	x[1] = cos(dec_in*M_PI/180)*sin(ra_in*M_PI/180);
	x[2] = sin(dec_in*M_PI/180);

	double ra_out,dec_out;

	Cartesian2Spherical_ra_dec_radian(x,ra_out,dec_out);

	ra_out  *= 180/M_PI;
	dec_out *= 180/M_PI;

	cout.precision(15);
	cout << "Input:  ra = " << setw(18) << ra_in 
		 << ", dec = " << setw(18) << dec_in << "\n";
	
	cout << "Output: ra = " << setw(18) << ra_out 
		 << ", dec = " << setw(18) << dec_out << "\n";

	cout << "diff_ra  = " << setw(25) << ra_out-ra_in
		 << "\ndiff_dec = " << setw(25) << dec_out-dec_in << "\n";
}