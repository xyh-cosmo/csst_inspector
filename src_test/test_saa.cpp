//	compared mine SAA algrithm to ZhangXin's implementation.
//	Result: perfectly matches!


#include "inspector.hpp"

using namespace std;

int main( int argc, char* argv[] ){

	int num_max = 10000;

	const gsl_rng_type *T = gsl_rng_default;
	gsl_rng *r = gsl_rng_alloc(T);

	SAA_Condition saa;

	ofstream of1("saa1.txt");
	ofstream of2("saa2.txt");
	of1.precision(10);
	of2.precision(10);

	for( int i=0; i<num_max; ++i ){
		double ra = gsl_ran_flat(r,-85,10);
		double dec= gsl_ran_flat(r,-45,-10);
		int flag1,flag2;

		flag1 = saa.test_saa1(ra,dec);
		flag2 = saa.test_saa2(ra,dec);

		if( flag1 == 0 )
			of1 << setw(15) << ra << "  " << setw(15) << dec << "\n";

		if( flag2 == 0 )
			of2 << setw(15) << ra << "  " << setw(15) << dec << "\n";
	}
    
	of1.close();
	of2.close();
}
