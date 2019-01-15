#include "inspector.hpp"

Ephemeris::Ephemeris(){
    jpl405 = NULL;
}

Ephemeris::Ephemeris(std::string& jpl504_filename){
    init(jpl504_filename);
}

Ephemeris::~Ephemeris(){
    if( jpl405 != NULL ){
        fclose(jpl405);
        jpl405 = NULL;
    }
}

int Ephemeris::init(std::string& jpl504_filename){
    jpl405 = fopen(jpl504_filename.c_str(),"r");
    if( jpl405 == NULL ){
        PrintError("Failed to open JPL405.");
    }
    else{
        std::cout << "\n==> Ephemeris: sucessfully loaded jpl405 data from: "
                  << jpl504_filename << "\n";
    }
    return 0;
}

void Ephemeris::purge(){
    if( jpl405 != NULL ){
        std::cout << "\n==> Purge JPL file handler ...\n";
        fclose(jpl405);
    }
}

int Ephemeris::get_sun_position(double time, double&x, double&y, double& z){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_SUN, EPHCOM_EARTH, testr);
        x = testr[0];
        y = testr[1];
        z = testr[2];
    } else {
        printf("Coordinates not found for %f \n",time);
        x = 0;
        y = 0;
        z = 0;
        status = 1;
    }

    free(datablock);
    return status;
}


int Ephemeris::get_sun_position(double time, double&x, double&y, double& z, double& dist){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_SUN, EPHCOM_EARTH, testr);
        x = testr[0];
        y = testr[1];
        z = testr[2];
    } else {
        printf("Coordinates not found for %f \n",time);
        x = 0;
        y = 0;
        z = 0;
        status = 1;
    }

    free(datablock);
    dist = sqrt(x*x+y*y+z*z);
    return status;
}


int Ephemeris::get_sun_position(double time, double sun[]){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_SUN, EPHCOM_EARTH, testr);
        sun[0] = testr[0];
        sun[1] = testr[1];
        sun[2] = testr[2];
    } else {
        printf("Coordinates not found for %f \n",time);
        sun[0] = 0;
        sun[1] = 0;
        sun[2] = 0;
        status = 1;
    }

    free(datablock);
    return status;
}


int Ephemeris::get_sun_position(double time, double sun[], double& dist){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_SUN, EPHCOM_EARTH, testr);
        sun[0] = testr[0];
        sun[1] = testr[1];
        sun[2] = testr[2];
    } else {
        printf("Coordinates not found for %f \n",time);
        sun[0] = 0;
        sun[1] = 0;
        sun[2] = 0;
        status = 1;
    }

    free(datablock);
    dist = sqrt(sun[0]*sun[0] + sun[1]*sun[1] + sun[2]*sun[2]);
    return status;
}

int Ephemeris::get_moon_position(double time, double&x, double&y, double& z){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_MOON, EPHCOM_EARTH, testr);
        x = testr[0];
        y = testr[1];
        z = testr[2];
        // status = 0;
    } else {
        printf("Coordinates not found for %f \n",time);
        x = 0;
        y = 0;
        z = 0;
        status = 1;
    }

    free(datablock);
    return status;
}

int Ephemeris::get_moon_position(double time, double&x, double&y, double& z, double& dist){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_MOON, EPHCOM_EARTH, testr);
        x = testr[0];
        y = testr[1];
        z = testr[2];
        // status = 0;
    } else {
        printf("Coordinates not found for %f \n",time);
        x = 0;
        y = 0;
        z = 0;
        status = 1;
    }

    free(datablock);
    dist = sqrt(x*x+y*y+z*z);
    return status;
}


int Ephemeris::get_moon_position(double time, double moon[]){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_MOON, EPHCOM_EARTH, testr);
        moon[0] = testr[0];
        moon[1] = testr[1];
        moon[2] = testr[2];
        // status = 0;
    } else {
        printf("Coordinates not found for %f \n",time);
        moon[0] = 0;
        moon[1] = 0;
        moon[2] = 0;
        status = 1;
    }

    free(datablock);
    return status;
}

int Ephemeris::get_moon_position(double time, double moon[], double& dist){
    int status=0;
    struct ephcom_Header header;
    struct ephcom_Coords coords;
    double *datablock; /* Will hold coefficients from a data block */
    // int datablocknum;
    double testr[6];
    ephcom_readbinary_header(jpl405, &header);
    // ephcom_writeascii_header(outfp, &header1);
    /*
     Done with header.  Now we'll read and write data blocks.
     */
    datablock = (double *) malloc((int) header.ncoeff * sizeof(double));
    // datablocknum = 0;
    coords.km = 1; /* not AU, use kilometers */
    coords.seconds = 0; /* Timescale is days, not seconds */
    coords.bary = 1; /* Center is Solar System Barycenter */
    coords.coordtype = 0; /* No correction for light travel time or
     relativistic effects from Sun */

    coords.et2[0] = time; /* Good enough precision for test dates */
    coords.et2[1] = 0.0;
    if (ephcom_get_coords(jpl405, &header, &coords, datablock) == 0) {
        ephcom_pleph(&coords, EPHCOM_MOON, EPHCOM_EARTH, testr);
        moon[0] = testr[0];
        moon[1] = testr[1];
        moon[2] = testr[2];
        // status = 0;
    } else {
        printf("Coordinates not found for %f \n",time);
        moon[0] = 0;
        moon[1] = 0;
        moon[2] = 0;
        status = 1;
    }

    free(datablock);
    dist = sqrt(moon[0]*moon[0] + moon[1]*moon[1] + moon[2]*moon[2]);
    return status;
}


void Ephemeris::run_jpl405_test(double t_start, double t_end, int steps){
    double t = t_start;
    double dt = (t_end-t_start)/(steps-1.0);
    double x,y,z,dist;

    while( t <= t_end ){
        if( get_sun_position(t,x,y,z) == 0 ){
            dist = sqrt(x*x+y*y+z*z);
            std::cout.precision(15);
            std::cout << "Sun: " 
                      << std::setw(18) << x << " "
                      << std::setw(18) << y << " "
                      << std::setw(18) << z << " "
                      << "D = " << std::setw(18) << dist << "  ";
        }

        if( get_moon_position(t,x,y,z) == 0 ){
            dist = sqrt(x*x+y*y+z*z);
            std::cout.precision(15);
            std::cout << "Moon: " 
                      << std::setw(18) << x << " "
                      << std::setw(18) << y << " "
                      << std::setw(18) << z << " "
                      << "D = " << std::setw(18) << dist << "\n";
        }
        t += dt;
    }
}