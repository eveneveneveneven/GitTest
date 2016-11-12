#include <iostream>
#include <iomanip>  
#include <fstream>
#include "ShipInfo.h"
#include "Eigen\Dense"
#include "CurrentModel.h"
using namespace Eigen;

struct Solver {
	long double epsilon, t, y, h, h_min, h_max;
	double a21, a31, a32, a41, a42, a43, a51, a52, a53, a54, a61, a62, a63, a64, a65, a71, a72, a73, a74, a75, a76, c2, c3, c4, c5, c6, c7, b11, b12, b13, b14, b15, b16, b17, b21, b22, b23, b24, b25, b26, b27;
	Vector3d k1v, k2v, k3v, k4v, k5v, k6v, k7v, zv, sv, errv;

}; Solver Ode45;

Solver FillOde45(Solver TempSolver);

Vector3d Calculate_Next_Nu(Solver Ode45, Ship TempShip);

Vector3d Calculate_Next_Eta(Solver Ode45, Ship TempShip);

double f(double t, double y);

Vector3d f_eta(double t, Vector3d eta, Ship CyberShipE1);

Vector3d f_nu(double t, Vector3d nu, Ship CyberShipE1);

double speed_control(double u, double u_ref);

double yaw_control(double psi, double psi_ref, double psi_dot);

Ship Get_Nonlinear_Matrices(Ship TempShip);

//double abs(double in);