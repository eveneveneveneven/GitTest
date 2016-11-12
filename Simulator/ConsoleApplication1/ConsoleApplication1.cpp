// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Matrix.h"
#include "Eigen/Dense"
#include "ShipInfo.h"

using namespace Eigen;

int main()
{
	
	Ship SimulatedShip = GetVesselData();
	//std::cout << SimulatedShip.M_inv << std::endl;
	Solve(100, SimulatedShip);
	system("pause");
    return 0;
}

Ship GetVesselData() {
	std::string line;
	std::ifstream myfile("Ships/JolnerTest.txt");
	std::vector<std::string> ParameterNames;
	std::string ShipName;
	std::vector<double> ParameterValues;
	std::vector<std::string> TempString;
	int i = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			//std::cout << line << std::endl;
			TempString = split(line, ',');
			ParameterNames.push_back(TempString[0]);
			if (i == 0) {
				ShipName = TempString[1];
			}
			ParameterValues.push_back(atof(TempString[1].c_str()));
			//std::cout << TempString[1].c_str() << std::endl;
			i++;

		}
		std::cout << "Vessel model for " << ShipName << " loaded correctly" << std::endl;
		myfile.close();
	}
	else std::cout << "Cannot find the specified vessel model...";
	Ship FoundShip;
	FoundShip.name = ShipName;
	FoundShip.m = ParameterValues[1];
	double T = ParameterValues[2];
	double B = ParameterValues[3];
	double L = ParameterValues[4];
	FoundShip.x_g = ParameterValues[5];
	//std::cout << T << std::endl;
	FoundShip.y_g = ParameterValues[6];
	FoundShip.z_g = ParameterValues[7];
	FoundShip.x_b = ParameterValues[8];
	FoundShip.y_b = ParameterValues[9];
	FoundShip.z_b = ParameterValues[10];
	double C_B = ParameterValues[11];
	FoundShip.l_1 = 0.1505;
	FoundShip.l_2 = -0.1505;
	FoundShip.L = L;
	FoundShip.Thrust_Config_Matrix.resize(3, 2);
	FoundShip.Thrust_Config_Matrix << 1, 1,
		0, 0,
		-FoundShip.l_1, -FoundShip.l_2;
	//std::cout << FoundShip.Thrust_Config_Matrix;
	double rho = 1025;
	double nabla = FoundShip.m / rho;
	FoundShip.X_u_dot = -FoundShip.m / (M_PI*sqrt((L*L*L / nabla) - 14));
	double Y_v_dot_nd = -M_PI*(T / L)*(T/L)* (1 + 0.16*C_B*(B / T) - 5.1*(B / L)*(B/L));
	double Y_r_dot_nd = -M_PI*(T / L)*(T / L)* (0.67*B / L - 0.0033*(B / T) *(B/T));
	double N_v_dot_nd = -M_PI*(T / L)*(T / L)* (1.1*B / L - 0.041*B / T);
	double N_r_dot_nd = -M_PI*(T / L)*(T / L)* (0.08333 + 0.017*C_B*B / T - 0.33*B / L);
	double Y_v_nd = -M_PI*(T / L)*(T / L)* (1 + 0.4*C_B*B / T);
	double Y_r_nd = -M_PI*(T / L)*(T / L)* (-0.5 + 2.2*B / L - 0.08*B / T);
	double N_v_nd = -M_PI*(T / L)*(T / L)* (0.5 + 2.4*T / L);
	double N_r_nd = -M_PI*(T / L)*(T / L)* (0.25 + 0.039*B / T - 0.56*B / L);
	double u = 0.5;
	double u_2 = 1;
	double T_sway = 1;
	double nd_d_m_a = rho*(u*u / 2)*L*L;
	double nd_d = rho*(u_2*u_2 / 2)*L*L;

	double Y_vv_cse, X_uu_cse, X_uuu_cse, Y_v_cse, N_v_cse, N_vv_cse, Y_rv_cse, Y_r_cse, Y_vr_cse, Y_rr_cse, N_rv_cse, N_r_cse, N_vr_cse, N_rr_cse;
	Y_vv_cse = -36.47;
	X_uu_cse = -1.32742;
	X_uuu_cse = -5.86643;
	Y_v_cse = -0.88965;
	N_v_cse = 0.03130;
	N_vv_cse = 3.95645;
	Y_rv_cse = -0.805;
	Y_r_cse = -7.25;
	Y_vr_cse = -0.845;
	Y_rr_cse = -3.45;
	N_rv_cse = 0.130;
	N_r_cse = -1.9;
	N_vr_cse = 0.08;
	N_rr_cse = -0.75;
	FoundShip.Y_v_dot = nd_d_m_a*Y_v_dot_nd;
	FoundShip.N_r_dot = nd_d_m_a*N_r_dot_nd*FoundShip.L;
	FoundShip.Y_r_dot = nd_d_m_a*Y_r_dot_nd;
	FoundShip.N_v_dot = nd_d_m_a*N_v_dot_nd*FoundShip.L;
	FoundShip.I_z = FoundShip.m*(0.25*FoundShip.L)*(0.25*FoundShip.L);
	FoundShip.M << FoundShip.m - FoundShip.X_u_dot, 0, 0,
		0, FoundShip.m - FoundShip.Y_v_dot, FoundShip.m * FoundShip.x_g,
		0, FoundShip.m * FoundShip.x_g, FoundShip.I_z - FoundShip.N_r_dot;
	//std::cout << FoundShip.M << std::endl;
	FoundShip.M_inv = FoundShip.M.inverse();
	
	
	FoundShip.Y_v = nd_d*Y_v_nd;
	FoundShip.X_u = FoundShip.Y_v / 10;// -(FoundShip.m*1.1) / 50;
	FoundShip.Y_r = nd_d*Y_r_nd;
	FoundShip.N_v = -nd_d*L*N_v_nd;
	FoundShip.N_r = nd_d*L*N_r_nd;
	FoundShip.Y_vv = -135.3;
	double ratio = FoundShip.Y_vv / Y_vv_cse;
	FoundShip.N_rr = ratio*N_rr_cse;
	FoundShip.N_rrr = 0;
	FoundShip.N_rv = N_rv_cse*ratio;
	FoundShip.N_vr = N_vr_cse*ratio;
	FoundShip.N_vv = N_vv_cse*ratio;
	FoundShip.N_vvv = 0;
	FoundShip.Y_rr = Y_rr_cse*ratio;
	FoundShip.Y_rrr = 0;
	FoundShip.Y_rv = Y_rv_cse*ratio;
	FoundShip.Y_vr = Y_vr_cse*ratio;
	FoundShip.Y_vvv = -0;
	FoundShip.X_uu = X_uu_cse*ratio;
	FoundShip.X_uuu = X_uuu_cse*ratio/3;
	FoundShip.X_v = 0;
	FoundShip.X_vv = 0;
	FoundShip.X_vvv = 0;
	//Initial values:
	FoundShip.nu << 0, 0, 0;
	FoundShip.eta << -2, 0, 0;
	FoundShip.tau_control << 0, 0, 0;
	FoundShip.tau_current << 0, 0, 0;
	FoundShip.tau_wind << 0, 0, 0;
	FoundShip.R << 1, 0, 0,
		0, 1, 0,
		0, 0, 1;
	
	return FoundShip;
}

//Ship GetVesselData() {
//	std::string line;
//	std::ifstream myfile("Ships/CSEnterprizeII.txt");
//	std::vector<std::string> ParameterNames;
//	std::string ShipName;
//	std::vector<double> ParameterValues;
//	std::vector<std::string> TempString;
//	int i = 0;
//	if (myfile.is_open())
//	{
//		while (getline(myfile, line))
//		{
//			TempString = split(line, ',');
//			ParameterNames.push_back(TempString[0]);
//			if (i == 0) {
//				ShipName = TempString[1];
//			}
//			ParameterValues.push_back(atof(TempString[1].c_str()));
//			i++;
//
//		}
//		std::cout << "Vessel model for " << ShipName << " loaded correctly" << std::endl;
//		myfile.close();
//	}
//	else std::cout << "Cannot find the specified vessel model...";
//	Ship FoundShip;
//	FoundShip.name = ShipName;
//	FoundShip.m = ParameterValues[1];
//	FoundShip.I_z = ParameterValues[2];
//	FoundShip.x_g = ParameterValues[3];
//	FoundShip.y_g = ParameterValues[4];
//	FoundShip.X_u_dot = ParameterValues[5];
//	FoundShip.Y_v_dot = ParameterValues[6];
//	FoundShip.Y_r_dot = ParameterValues[7];
//	FoundShip.N_r_dot = ParameterValues[8];
//	FoundShip.X_u = ParameterValues[9];
//	FoundShip.X_uu = ParameterValues[10];
//	FoundShip.X_uuu = ParameterValues[11];
//	FoundShip.X_v = ParameterValues[12];
//	FoundShip.X_vv = ParameterValues[13];
//	FoundShip.X_vvv = ParameterValues[14];
//	FoundShip.Y_v = ParameterValues[15];
//	FoundShip.Y_vv = ParameterValues[16];
//	FoundShip.Y_vvv = ParameterValues[17];
//	FoundShip.Y_r = ParameterValues[18];
//	FoundShip.Y_rr = ParameterValues[19];
//	FoundShip.Y_rrr = ParameterValues[20];
//	FoundShip.Y_rv = ParameterValues[21];
//	FoundShip.Y_vr = ParameterValues[22];
//	FoundShip.N_v = ParameterValues[23];
//	FoundShip.N_vv = ParameterValues[24];
//	FoundShip.N_vvv = ParameterValues[25];
//	FoundShip.N_r = ParameterValues[26];
//	FoundShip.N_rr = ParameterValues[27];
//	FoundShip.N_rrr = ParameterValues[28];
//	FoundShip.N_rv = ParameterValues[29];
//	FoundShip.N_vr = ParameterValues[30];
//	FoundShip.l_1 = 0.1505;
//	FoundShip.l_2 = -0.1505;
//	//std::cout << FoundShip.l_1 << std::endl;
//	FoundShip.Thrust_Config_Matrix.resize(3, 2);
//	FoundShip.Thrust_Config_Matrix << 1, 1,
//		0, 0,
//		-FoundShip.l_1, -FoundShip.l_2;
//	//std::cout << FoundShip.Thrust_Config_Matrix;
//	FoundShip.M << FoundShip.m - FoundShip.X_u_dot, 0, 0,
//		0, FoundShip.m - FoundShip.Y_v_dot, FoundShip.m * FoundShip.x_g - FoundShip.Y_r_dot,
//		0, FoundShip.m * FoundShip.x_g - FoundShip.Y_r_dot, FoundShip.I_z - FoundShip.N_r_dot;
//	FoundShip.M_inv = FoundShip.M.inverse();
//	
//	//Initial values:
//	FoundShip.nu << 0, 0, 0;
//	FoundShip.eta << 0, 0, 0;
//	FoundShip.tau_control << 0, 0, 0;
//	FoundShip.tau_current << 0, 0, 0;
//	FoundShip.tau_wind << 0, 0, 0;
//	FoundShip.R << 1, 0, 0,
//		0, 1, 0,
//		0, 0, 1;
//
//	return FoundShip;
//}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}