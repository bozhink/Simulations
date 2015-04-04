/* 
 * File:   main.cpp
 * Author: Bozhin Karaivanov
 *
 * Created on September 18, 2012
 *
 * This program simulates the Fermi-Pasta-Ulam (FPU) model:
 * 1D chain of masses (m=1) connected by linear springs (k=1) with
 * a small (alpha) nonlinearity (quadratic in the force)
 */
/*
 * Original program:
 *  
 *   fermi.c
 *   by N Giordano -- July, 2006
 * 
 */

#include "fpu.h"

FPUModel::FPUModel(int _nParticles, int _modeInit, double _amplitude,
		double _dt, double _tMax, int _nModes, double _alpha, double _beta) {
	int modeInit;
	double omega1;
	double amplitude, lambda;

	nParticles = _nParticles;
	nParticles1 = nParticles + 1;
	d2n = 2.0 / nParticles1;
	sqd2n = sqrt(d2n);

	modeInit = _modeInit;
	amplitude = _amplitude;

	// string starts from rest
	lambda = 2.0 / (modeInit);

	for (int i = 1; i <= nParticles; i++) {
		x[i] = xOld[i] = amplitude * sqd2n * sin(PI * i * d2n / (lambda));
	}
	x[0] = xOld[0] = 0.0;
	x[nParticles1] = xOld[nParticles1] = 0.0;

	omega1 = PI / (nParticles1); // frequency of the lowest mode
	dt = _dt;

	r2 = dt * dt;
	tMax = _tMax;
	nModes = _nModes;
	tRecord = 20 * dt;

	beta = _beta;
	betaPrime = beta * dt * dt;

	alpha = _alpha;
	alphaPrime = alpha * dt * dt;

	// Open files
	status = true;
	char fName[80];

	fTotalE.open(TOTALE_FILE);
	if (!fTotalE) {
		ErrorOpenFile(TOTALE_FILE);
		status = false;
	}
	fX.open(XT_FILE);
	if (!fX) {
		ErrorOpenFile(XT_FILE);
		status = false;
	}
	fForce.open(FORCET_FILE);
	if (!fForce) {
		ErrorOpenFile(FORCET_FILE);
		status = false;
	}
	fPhase1.open(PHASEPLOT1_FILE);
	if (!fPhase1) {
		ErrorOpenFile(PHASEPLOT1_FILE);
		status = false;
	}
	fPhase2.open(PHASEPLOT2_FILE);
	if (!fPhase2) {
		ErrorOpenFile(PHASEPLOT2_FILE);
		status = false;
	}
	fProfile.open(PROFILE_FILE);
	if (!fProfile) {
		ErrorOpenFile(PROFILE_FILE);
		status = false;
	}
	fMode.open(MODE_FILE);
	if (!fMode) {
		ErrorOpenFile(MODE_FILE);
		status = false;
	}
	fXMode.open(XMODE_FILE);
	if (!fXMode) {
		ErrorOpenFile(XMODE_FILE);
		status = false;
	}

	SaveProfile();
	cout << "FPU Model:\nNumber of particles = " << nParticles << '\n';
	cout << "alpha = " << alpha << '\n';
	cout << "beta  = " << beta << '\n';
}

void FPUModel::SaveProfile() {
	for (int i = 0; i <= nParticles1; i++) {
		fProfile << i << '\t' << x[i] << endl;
	}
}

FPUModel::~FPUModel() {
	fTotalE.close();
	fX.close();
	fForce.close();
	fPhase1.close();
	fPhase2.close();
	fProfile.close();
	fMode.close();
	fXMode.close();
}

void FPUModel::CalculateNewPositions() {
	double X, Y;
	for (int i = 1; i <= nParticles; i++) {
		X = x[i + 1] - x[i];
		Y = x[i] - x[i - 1];
		xNew[i] = r2 * (X - Y) + alphaPrime * (X * X - Y * Y)
				+ betaPrime * (X * X * X - Y * Y * Y) + 2.0 * x[i] - xOld[i];
	}
	for (int i = 1; i <= nParticles; i++) {
		xOld[i] = x[i];
		x[i] = xNew[i];
	}
	return;
}

double FPUModel::TotalEnergy() {
	double v, dx;
	double sum = 0.0;
	for (int i = 1; i <= nParticles1; i++) {
		v = (x[i] - xOld[i]) / dt;
		dx = 0.5 * (x[i] + xOld[i] - (x[i - 1] + xOld[i - 1]));
		sum += 0.5 * (v * v + dx * dx);
	}
	return sum;
}

void FPUModel::GetA(int k, double*a, double*b) {
	double arg = k * PI / nParticles1;
	*a = 0.0;
	*b = 0.0;
	for (int i = 1; i <= nParticles; i++) {
		*a += x[i] * sin(i * arg);
		*b += xOld[i] * sin(i * arg);
	}
}

double FPUModel::ModeEnergy(int k, int iflag) {
	double T, V;
	if (iflag == 1) {
		GetA(k, &a, &aOld);
	}
	T = (a - aOld) * sqd2n / dt;
	V = (a + aOld) * sqd2n * sin(PI * k / (2 * nParticles1));
	return (0.5 * (T * T + V * V));
}

double FPUModel::ModePosition(int k, int iflag) {
	if (iflag == 1) {
		GetA(k, &a, &aOld);
	}
	return (0.5 * (a + aOld) * sqd2n);
}

double FPUModel::ModeVelocity(int k, int iflag) {
	if (iflag == 1) {
		GetA(k, &a, &aOld);
	}
	return ((a - aOld) * sqd2n / dt);
}

bool FPUModel::ModeCross(int k, int iflag) {
	if (iflag == 1) {
		GetA(k, &a, &aOld);
	}
	if (a * aOld > 0) {
		return false;
	} else {
		return true;
	}
}

void FPUModel::Run() {
	int i;
	double t = 0;
	double tTest = tRecord;
	double X, Y;
	while (t < tMax) {

		CalculateNewPositions();

		t += dt;
		tTest -= dt;
		if (tTest <= 0) {
			tTest = tRecord;
			/*
			 * Writing the total energy to file
			 */
			fTotalE << t << '\t' << TotalEnergy() << endl;
			/*
			 * Writing x(t) to file
			 */
			fX << t << '\t' << x[nParticles / 3] << endl;
			/*
			 * Writing information about the modes to file
			 */
			fMode << t;
			fXMode << t;
			for (i = 1; i <= nModes; i++) {
				// i begins from 1 because (i=0)-th mode has zero energy & position
				fMode << '\t' << ModeEnergy(i, 1);
				fXMode << '\t' << ModePosition(i, 0);
			}
			fMode << endl;
			fXMode << endl;
			/*
			 * Writing information about the force to file
			 */
			X = x[i + 1] - x[i];
			Y = x[i] - x[i - 1];
			fForce << t << '\t' << X - Y // Linear part of the force
			<< '\t' << alpha * (X * X - Y * Y) // Quadratic non-linearity
			<< '\t' << beta * (X * X * X - Y * Y * Y) // Cubic non-linearity
			<< endl;
			/*
			 * Writing phase portraits
			 */
			double mp = ModePosition(1, 0);
			double mv = ModeVelocity(1, 0);
			fPhase1 << mp << '\t' << mv << endl;
			if (ModeCross(3, 0)) {
				fPhase2 << mp << '\t' << mv << endl;
			}
		}
	}
}
