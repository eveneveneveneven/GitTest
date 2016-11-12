#include "stdafx.h"
#include "CurrentModel.h"

double GenerateRandomV() {
	std::random_device rd;
	std::mt19937 gen(rd());
	double mean, stdDev;
	mean = 0;
	stdDev = 0.10;
	static std::normal_distribution<> d(mean, stdDev);
	double res = d(gen);
	return res;
}

double GenerateRandomBeta() {
	std::random_device rd;
	std::mt19937 gen(rd());
	double mean, stdDev;
	mean = 0;
	stdDev = 1;
	static std::normal_distribution<> d(mean, stdDev);
	double res = d(gen);
	return res;
}

double updateCurrentSpeed(double ts, double V, double mu){
	double w = GenerateRandomV();
	double V_dot = w - mu*V;
	V = V + ts*V_dot;
	return V;
}

double updateCurrentDir(double ts, double beta, double mu) {
	double w = GenerateRandomBeta();
	double beta_dot = w - mu*beta;
	beta = beta + ts*beta_dot;
	return beta;
}