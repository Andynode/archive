/** IZP Project 2
 * Ondrej Sedlacek [xsedla1o], Autumn 2019
 *
 * Iterative calculation
 *  Input: 
 *   Values of U0, R and EPS describing a circuit with a DC voltage source
 *   connected to a resistor connected to a diode connected to ground.
 *   Values are given as args.
 *
 *  Output: 
 *   Voltage and current on the diode described by IO and UT, printed to stdin.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Definitions
#define I0 1e-12
#define UT 25.8563e-3

#define ERR 1
#define OK 0


// Function Declarations
int loadArgs(int argc, char **argv, double *u0, double *r, double *eps);
double diode(double u0, double r, double eps);
double solveShockley(double u0, double r, double up);


// Main program
int main(int argc, char **argv){
    // init
    double u0, r, eps, up, ip;

    // check and load args
    if(loadArgs(argc, argv, &u0, &r, &eps) != OK)
        return 1;

    // calculate
    up = diode(u0, r, eps);
    ip = I0 * (exp(up/UT) - 1);

    // output
    printf("Up=%g V\nIp=%g A\n", up, ip);

    // end
    return 0;
}


/**
 * @brief Loads args, prints any errors encountered
 * @return OK on success, ERR on failure
 */
int loadArgs(int argc, char **argv, double *u0, double *r, double *eps){
    if(argc != 4){
        fprintf(stderr,"Error: Wrong number of arguments\n");
        fprintf(stderr,"Expected 3 arguments in this order: U0 R EPS\n");
        return ERR;
    }

    char *end;
    *u0 = strtod(argv[1], &end);
    if(*end != '\0'){
        fprintf(stderr,"Error: Reached unexpected chars when parsing U0.\n");
        return ERR;
    }
    if(*u0 < 0){
        fprintf(stderr,"Error: Invalid arguments\n");
        return ERR;
    }

    *r = strtod(argv[2], &end);
    if(*end != '\0'){
        fprintf(stderr,"Error: Reached unexpected chars when parsing R.\n");
        return ERR;
    }
    if(*r < 0){
        fprintf(stderr,"Error: Invalid arguments\n");
        return ERR;
    }

    *eps = strtod(argv[3], &end);
    if(*end != '\0'){
        fprintf(stderr,"Error: Reached unexpected chars when parsing EPS.\n");
        return ERR;
    }
    if(*eps < 0){
        fprintf(stderr,"Error: Invalid arguments\n");
        return ERR;
    }

    return OK;
}


/**
 * @brief Uses bisection to calculate voltage on diode in given conditions
 * @return U_p voltage on diode
 */
double diode(double u0, double r, double eps){
    double start = 0;
    double end = u0;

    double mid = (start + end) / 2;
    double fmid = solveShockley(u0,r,mid);

    double oldMid = 0;

    while(fabs(start - end) > eps && mid != oldMid)
    {
        //printf("%.20g\t%.20g\t%.20g\n", start, mid, end);
        //printf("%g %g\n", fmid, I0 * (exp(mid/UT) - 1));
        if(fmid > 0)
            end = mid;
        else
            start = mid;
        oldMid = mid;
        mid = (start + end) / 2;
        fmid = solveShockley(u0,r,mid);
    }

    return mid;
}


/**
 * @return ip - ir, the closer to 0, the better
 */
double solveShockley(double u0, double r, double up){
    return I0 * (exp(up / UT) - 1) - (u0 - up)/r;
}
