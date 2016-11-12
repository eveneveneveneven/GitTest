#include "Eigen\Dense"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma once
using namespace Eigen;


struct Ship {
	Matrix3d M, C, D, R, M_inv;
	Vector3d eta, nu, nu_r, nu_c_b, nu_c_n, tau_control, tau_wind, tau_current;
	MatrixXd Thrust_Config_Matrix;
	int test;
	std::string name;
	double current_speed, current_direction, mu_v, w_v, mu_beta, w_beta, m, I_z, x_g, y_g, X_u_dot, Y_v_dot, Y_r_dot, N_v_dot, N_r_dot, X_u, X_uu, X_uuu, X_v, X_vv, X_vvv, Y_v, Y_vv, Y_vvv, Y_r, Y_rr, Y_rrr, Y_rv, Y_vr, N_v, N_vv, N_vvv, N_r, N_rr, N_rrr, N_rv, N_vr, l_1, l_2, K_1, K_2, C_B, z_g, x_b, y_b, z_b, T, B, L;
};
Ship GetVesselData();

std::vector<std::string> split(const std::string &s, char delim);

void split(const std::string &s, char delim, std::vector<std::string> &elems);

std::string GetGPSData(double Lat, double Long, double u, double v);

std::string GetHeadingString(double HeadingRad);