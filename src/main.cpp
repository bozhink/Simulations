/* 
 * File:   main.cpp
 * Author: Bozhin Karaivanov
 *
 * Created on September 18, 2012, 11:00 PM
 *
 * This program simulates the Fermi-Pasta-Ulam (FPU) model:
 * 1D chain of masses (m=1) connected by linear springs (k=1) with
 * a small (alpha) nonlinearity (quadratic in the force)
 */
/*
 * Original program, wich was used:
 *  
 *   fermi.c
 *   by N Giordano -- July, 2006
 * 
 */

#include "fpu.h"

/*
 * 
 */
int main(int argc, char** argv) {
    int nParticles = 32;     //  there are 32 particles that can move -
                             //  with the end particles connected to rigid
                             //  boundary points
    int modeInit = 1;        // mode that contains all of the initial energy
                             // mode #1 is the lowest mode
    double amplitude = 10.0; // initial amplitude
    double dt = 0.05;
    int tMax = 100000;
    int nModes = 10;  // number of modes to track
    
    double alpha, beta;
    // different values of beta and alpha of interest
    beta = 1;     // non-chaotic
    beta = 3;     // seems chaotic
    beta = 0;
    beta = 0;
    beta = 3;     // seems chaotic
    beta = 6;
    beta = 1;     // non-chaotic
    beta = 0.3;

    alpha = 1.0;
    alpha = 0.3;
    alpha = 0.6;
    alpha = 0.25;
    alpha = 0;

    FPUModel fpu(nParticles, modeInit, amplitude, dt, tMax, nModes, alpha, beta);
    if (fpu.status) {
        fpu.Run();
    } else {
        cerr << "Some file(s) was(were) not created.\n";
        cerr << "Program " << argv[0] << "will stop now.";
        return 1;
    }
    
    return 0;
}

