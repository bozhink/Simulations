/* 
 * File:   fpu.h
 * Author: bozhin
 *
 * Created on September 18, 2012, 11:04 PM
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

#ifndef FPU_H
#define	FPU_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

#define PI 3.1415926535897932

#ifndef MAX
#define MAX 100
#endif

#ifndef TOTALE_FILE
#define TOTALE_FILE "total_e"
#endif
#ifndef XT_FILE
#define XT_FILE "x_t"
#endif
#ifndef FORCET_FILE
#define FORCET_FILE "force_t"
#endif
#ifndef PHASEPLOT1_FILE
#define PHASEPLOT1_FILE "phase_plot_1"
#endif
#ifndef PHASEPLOT2_FILE
#define PHASEPLOT2_FILE "phase_plot_2"
#endif
#ifndef MODE_FILE
#define MODE_FILE "mode"
#endif
#ifndef XMODE_FILE
#define XMODE_FILE "x_mode"
#endif
#ifndef PROFILE_FILE
#define PROFILE_FILE "prof.junk"
#endif

class FPUModel {
    void CalculateNewPositions ();
    void SaveProfile ();
    bool ModeCross (int, int);
    void GetA (int, double*, double*);
    
    double ModeEnergy (int, int);
    double TotalEnergy ();
    double ModePosition (int, int);
    double ModeVelocity (int, int);
    
    double x[MAX],xOld[MAX],xNew[MAX];
    int nParticles,nParticles1;  // number of particles in the chain
    double alpha;                //size of nonlinearity - same for all masses
    double alphaPrime;           // normalized alpha
    double beta,betaPrime;

    double dt;       // time step
    double r2;       // used for iteration
    double tMax;     // length of simulation
    double tRecord;

    double d2n;
    double sqd2n;

    double a,aOld;

    int nModes;

    ofstream fMode;
    ofstream fXMode;
    ofstream fTotalE;
    ofstream fX;
    ofstream fForce;
    ofstream fPhase1;
    ofstream fPhase2;
    ofstream fProfile;
    void ErrorOpenFile (string fileName) {
        cerr << "FPUModel: Cannot open file " << fileName << endl;        
    }
    
public:
    bool status;
    FPUModel (int, int, double, double, double, int ,double, double);
    ~FPUModel ();
    void Run ();
};

#endif	/* FPU_H */

