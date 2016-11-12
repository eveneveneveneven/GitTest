#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

double GenerateRandomV();
double GenerateRandomBeta();
double updateCurrentSpeed(double ts, double V, double mu);
double updateCurrentDir(double ts, double beta, double mu);