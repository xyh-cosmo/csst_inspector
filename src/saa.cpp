#include "inspector.hpp"

SAA_Condition::SAA_Condition(){
    rangePoint[0][0] = -13;
    rangePoint[0][1] = -42.5;

    rangePoint[1][0] = -72;
    rangePoint[1][1] = -42.5;

    rangePoint[2][0] = -81;
    rangePoint[2][1] = -25;

    rangePoint[3][0] = -54;
    rangePoint[3][1] = -15;

    rangePoint[4][0] =  5;
    rangePoint[4][1] = -32;

    area_tolerance = 0;
    area_min = compute_area_min();
}

void SAA_Condition::set_area_tolerance(double tol){

    if( std::fabs(tol) > 1e-3 ){
        std::string errmsg="";
        double tmp = std::fabs(tol);
        errmsg += " std::fabs(tol) = " + Double2String(tmp) + " may be to large, choose a smaller one!";
        PrintError(errmsg);
    }

    area_tolerance = tol;
}

double SAA_Condition::compute_area_min(){
    double ra_tmp = 0, dec_tmp = 0;
    for( int i=0; i<5; ++i ){
        ra_tmp  += 0.2*rangePoint[i][0];
        dec_tmp += 0.2*rangePoint[i][1];
    }
    double uderStarPoint[2] = {ra_tmp,dec_tmp};
    
    area_min = 0;
    double a1,a2,b1,b2;
    for ( int i = 0; i < 5; ++i ) {
        a1 = uderStarPoint[0] - rangePoint[i][0];
        a2 = uderStarPoint[1] - rangePoint[i][1];
        int next = i + 1;

        if (next > 4) {
            next = next - 5;
        }

        b1 = rangePoint[next][0] - rangePoint[i][0];
        b2 = rangePoint[next][1] - rangePoint[i][1];
        area_min += std::fabs(a1 * b2 - a2 * b1);
    }

    // std::cout << "area_min = " << area_min << "\n";

    return area_min;
}


int SAA_Condition::test_saa1(double& ra, double& dec){

    double uderStarPoint[2] = {ra,dec};

    int newFlag = 1;
    int i, next;
    double directFlag[5];
    double a1,a2,b1,b2;

    for ( i = 0; i < 5; i++ ) {
        a1 = uderStarPoint[0] - rangePoint[i][0];
        a2 = uderStarPoint[1] - rangePoint[i][1];
        next = i + 1;

        if (next > 4) {
            next = next - 5;
        }

        b1 = rangePoint[next][0] - rangePoint[i][0];
        b2 = rangePoint[next][1] - rangePoint[i][1];
        directFlag[i] = a1 * b2 - a2 * b1;
    }

    for ( i = 0; i < 5; i++ ) {
        next = i + 1;
        if ( next > 4 ) {
            next = next - 5;
        }
        if ( directFlag[i] * directFlag[next] < 0 ) {
            newFlag = 0;
            break;
        }
    }

    return newFlag;
}

int SAA_Condition::test_saa2(double& ra, double& dec){

    double uderStarPoint[2] = {ra,dec};

    int newFlag = 1;
    int i, next;
    double a1,a2,b1,b2;
    double area = 0;

    for ( i = 0; i < 5; i++ ) {
        a1 = uderStarPoint[0] - rangePoint[i][0];
        a2 = uderStarPoint[1] - rangePoint[i][1];
        next = i + 1;

        if (next > 4) {
            next = next - 5;
        }

        b1 = rangePoint[next][0] - rangePoint[i][0];
        b2 = rangePoint[next][1] - rangePoint[i][1];
        area += std::fabs(a1 * b2 - a2 * b1);

        if( area >= area_min ){
            newFlag = 0;
            break;
        }
    }

    return newFlag;
}

int SAA_Condition::check(double& ra_ref, double& dec_ref){

    double uderStarPoint[2] = {ra_ref,dec_ref};

    int newFlag = 1;
    int i, next;
    double a1,a2,b1,b2;
    double area = 0;

    for ( i = 0; i < 5; i++ ) {
        a1 = uderStarPoint[0] - rangePoint[i][0];
        a2 = uderStarPoint[1] - rangePoint[i][1];
        next = i + 1;

        if (next > 4) {
            next = next - 5;
        }

        b1 = rangePoint[next][0] - rangePoint[i][0];
        b2 = rangePoint[next][1] - rangePoint[i][1];
        area += std::fabs(a1 * b2 - a2 * b1);

        if( area >= area_min*(1.0-area_tolerance) ){
            newFlag = 0;
            break;
        }
    }

    return newFlag;
}