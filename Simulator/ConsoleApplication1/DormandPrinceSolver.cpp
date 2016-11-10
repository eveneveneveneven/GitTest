#include "stdafx.h"
#include "Ode45.h"
#include "ShipInfo.h"
#include "UDPbroadcastToGUI.h"
#include <math.h>


double speed_control(double u, double SpeedRef) {
	double Error = SpeedRef - u;
	double SpeedOut = 0;
	static double LastSpeedRef = 0;
	double Ki = 32;
	double Kp = 96;
	static double IntegratedError = 0;

	LastSpeedRef = SpeedRef;
	double dt = 0.05;
	SpeedOut = Kp*Error + Ki*IntegratedError;
	IntegratedError = IntegratedError + Error*dt;
	//std::cout << "Speed Out " << SpeedOut << std::endl;
	return SpeedOut;
}

double yaw_control(double psi, double psi_ref, double psi_dot) {
	double Error = psi_ref - psi;
	double emulatedRudder = 0;
	while (1) { // Get heading from 0-2pi
		if (psi > 2 * M_PI) {
			psi = psi - 2 * M_PI;
		}
		else if (psi < 0) {
			psi = psi + 2 * M_PI;
		}
		else {
			break;
		}
	}
	double Ki = 0.1;
	double Kp = 60;
	double Kd = 20;
	static double IntegratedError = 0;
	double dt = 0.05;

	emulatedRudder = Kp*Error + Ki*IntegratedError - Kd*psi_dot;
	if (emulatedRudder > 60) {
		emulatedRudder = 60;
	}
	else if (emulatedRudder < -60) {
		emulatedRudder = -60;
	}
	IntegratedError = IntegratedError + Error*dt;
	//std::cout << "Speed Out " << SpeedOut << std::endl;
	return emulatedRudder;
}

void Solve(double tf, Ship CyberShipE1) {
	broadcastSocket bcSocket = UDPbroadCastSocketInit(GPS_UDP_PORT);
	broadcastSocket bcSocketHeading = UDPbroadCastSocketInit(CONTROLLER_UDP_PORT);
	std::string HeadingData;
	MatrixXd ThrustMatrix(2,1);
	const char *heading = "";
	double Lat, Long, LatGps, LongGps;
	FillOde45(Ode45);
	double Thruster_1, Thruster_2;
	std::ofstream resWrite;
	resWrite.open("res.txt");
	resWrite.precision(9);
	resWrite.setf(std::ios::fixed, std::ios::floatfield);
	std::cout << "Simulation in progress, simulating " << tf << " seconds..." << std::endl;
	double thrust_yaw = 0;
	double thrust_surge = 0;
	double thrust_sway = 0;
	double surge_ref = 1.11; //Reference speed, m/s
	double psi_ref = (M_PI / 180) * 0;
	CyberShipE1.current_speed = 0.2; //Current speed in m/s
	CyberShipE1.current_direction = 90; //Current direction in NED, in degrees.
	CyberShipE1.nu_c_n << CyberShipE1.current_speed*cos((M_PI / 180)*CyberShipE1.current_direction), CyberShipE1.current_speed*sin((M_PI / 180)*CyberShipE1.current_direction), 0;
	CyberShipE1.nu_c_b << CyberShipE1.current_speed*cos((M_PI / 180)*CyberShipE1.current_direction - CyberShipE1.eta(2)), CyberShipE1.current_speed*sin((M_PI / 180)*CyberShipE1.current_direction - CyberShipE1.eta(2)), 0;
	CyberShipE1.nu_r = CyberShipE1.nu - CyberShipE1.nu_c_b;
	std::cout << std::fixed << std::setprecision(3);
	std::clock_t end = clock();
	std::clock_t begin = clock();
	while (Ode45.t <= tf) {
		begin = clock();


		Sleep(48);
		thrust_surge = speed_control(CyberShipE1.nu(0), surge_ref);
		thrust_yaw = yaw_control(CyberShipE1.eta(2), psi_ref, CyberShipE1.nu(2)); // -20 * cos(0.15*Ode45.t);
		if (Ode45.t > 15) {
			psi_ref = (M_PI / 180) * 0;
		}
		if (Ode45.t > 30) {
			psi_ref = (M_PI / 180) * 90;
		}
		if (Ode45.t > 45) {
			psi_ref = (M_PI / 180) * 180;
		}
		if (Ode45.t > 60) {
			psi_ref = (M_PI / 180) * 270;
		}
		/*if (Ode45.t < 10) {
			thrust_yaw = 0;
			thrust_surge = 100;
		}
		if (Ode45.t >= 10) {
			thrust_surge = 0;
		}
		if (Ode45.t >= 15) {
			thrust_surge = 0;
		}
		if (Ode45.t >= 20 && Ode45.t<30) {
			thrust_yaw = 0;
		}*/
		CyberShipE1.nu_c_n << CyberShipE1.current_speed*cos((M_PI / 180)*CyberShipE1.current_direction), CyberShipE1.current_speed*sin((M_PI / 180)*CyberShipE1.current_direction), 0;
		CyberShipE1.nu_c_b << CyberShipE1.current_speed*cos((M_PI / 180)*CyberShipE1.current_direction - CyberShipE1.eta(2)), CyberShipE1.current_speed*sin((M_PI / 180)*CyberShipE1.current_direction - CyberShipE1.eta(2)), 0;
		CyberShipE1.nu = CyberShipE1.nu_r + CyberShipE1.nu_c_b;
		//std::cout << CyberShipE1.nu_r << ", \n" << CyberShipE1.nu << std::endl;

		Thruster_1 = thrust_surge;
		Thruster_2 = thrust_yaw;
		ThrustMatrix << Thruster_1, Thruster_2;
		CyberShipE1.tau_control << thrust_surge, thrust_sway, thrust_yaw;// CyberShipE1.Thrust_Config_Matrix*ThrustMatrix;//<< 2, 0, 0.3*cos(0.1* Ode45.t);
		std::string GpsData = GetGPSData(CyberShipE1.eta(0), CyberShipE1.eta(1), CyberShipE1.nu(0), CyberShipE1.nu(1));
		const char *data = GpsData.c_str();
		UDPbroadcastString(bcSocket, data);
		HeadingData=GetHeadingString(CyberShipE1.eta(2)*(180 / M_PI));
		heading = HeadingData.c_str();
		UDPbroadcastControlParams(bcSocketHeading, thrust_surge ,thrust_yaw,psi_ref*(180/M_PI),surge_ref*3.6, CyberShipE1.eta(2)*(180 / M_PI));
		resWrite << CyberShipE1.eta(0) << "   \t" << CyberShipE1.eta(1) << "   \t" << CyberShipE1.eta(2)<< "   \t" << CyberShipE1.nu(0) << "   \t" << CyberShipE1.nu(1) << "   \t" << CyberShipE1.nu(2) << "   \t" << CyberShipE1.tau_control(0) << "   \t" << CyberShipE1.tau_control(1) << "   \t" << CyberShipE1.tau_control(2) << "   \t" << Ode45.t << "\n";
		CyberShipE1.eta = Calculate_Next_Eta(Ode45, CyberShipE1);
		CyberShipE1.nu_r = Calculate_Next_Nu(Ode45, CyberShipE1);
		Ode45.t = Ode45.t + Ode45.h;
		end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << "\r Elapsed time per cycle: " << elapsed_secs << " seconds";
	}
	std::cout << "Simulation done" << std::endl; 
}

std::string GetHeadingString(double HeadingRad) {
	std::ostringstream strs;
	strs << HeadingRad;
	return "$Heading," + strs.str();
}

std::string GetGPSData(double Lat, double Long, double u, double v) {
	std::ostringstream strLat;
	strLat << Lat;
	std::string LatString = strLat.str();
	std::ostringstream strLong;
	strLong << Long;
	std::string LongString = strLong.str();
	double speedKmh = sqrt(u*u + v*v)*3.6;
	std::ostringstream strSpeed;
	strSpeed << speedKmh;
	std::string SpeedString = strSpeed.str();
	return "$GPGGA, 123519, "+LatString+", N, "+LongString+", E, 1, 08, 0.9, 545.4, M, 46.9, M, , *47 $GPVTG,054.7,T,034.4,M,005.5,N," + SpeedString +",K*48";
}

Solver FillOde45(Solver TempSolver) {
	Ode45.epsilon = 0.000001;
	Ode45.t = 0L;
	Ode45.y = 0L;
	Ode45.h = 0.05L;
	Ode45.h_min = 0.05L;
	Ode45.h_max = 0.05L;
	Ode45.a21 = 1.0L / 5L;
	Ode45.a31 = 3.0L / 40;
	Ode45.a32 = 9.0L / 40;
	Ode45.a41 = 44.0L / 45;
	Ode45.a42 = -56.0L / 15;
	Ode45.a43 = 32.0L / 9;
	Ode45.a51 = 19372.0L / 6561;
	Ode45.a52 = -25360.0L / 2187;
	Ode45.a53 = 64448.0L / 6561;
	Ode45.a54 = -212.0L / 729;
	Ode45.a61 = 9017.0L / 3168;
	Ode45.a62 = -355.0L / 33;
	Ode45.a63 = 46732.0L / 5247;
	Ode45.a64 = 49.0L / 176;
	Ode45.a65 = -5103.0L / 18656;
	Ode45.a71 = 35.0L / 384;
	Ode45.a72 = 0;
	Ode45.a73 = 500.0L / 1113;
	Ode45.a74 = 125.0L / 192;
	Ode45.a75 = -2187.0L / 6784;
	Ode45.a76 = 11.0L / 84;
	Ode45.c2 = 1.0L / 5;
	Ode45.c3 = 3.0L / 10;
	Ode45.c4 = 4.0L / 5;
	Ode45.c5 = 8.0L / 9;
	Ode45.c6 = 1.0L;
	Ode45.c7 = 1.0L;
	Ode45.b11 = 35.0L / 384;
	Ode45.b12 = 0.0L;
	Ode45.b13 = 500.0L / 1113;
	Ode45.b14 = 125.0L / 192;
	Ode45.b15 = -2187.0L / 6784;
	Ode45.b16 = 11.0L / 84;
	Ode45.b17 = 0.0L;
	Ode45.b21 = 5179.0L / 57600;
	Ode45.b22 = 0.0L;
	Ode45.b23 = 7571.0L / 16695;
	Ode45.b24 = 393.0L / 640;
	Ode45.b25 = -92097.0L / 339200;
	Ode45.b26 = 187.0L / 2100;
	Ode45.b27 = 1.0L / 40;

	return TempSolver;

}

Vector3d Calculate_Next_Nu(Solver Ode45, Ship TempShip) {
	Ode45.k1v = Ode45.h*f_nu(Ode45.t, TempShip.nu_r, TempShip);
	Ode45.k2v = Ode45.h*f_nu((Ode45.t + Ode45.c2*Ode45.h), (TempShip.nu_r + Ode45.a21*Ode45.k1v), TempShip);
	Ode45.k3v = Ode45.h*f_nu((Ode45.t + Ode45.c3*Ode45.h), (TempShip.nu_r + Ode45.a31*Ode45.k1v + Ode45.a32*Ode45.k2v), TempShip);
	Ode45.k4v = Ode45.h*f_nu((Ode45.t + Ode45.c4*Ode45.h), (TempShip.nu_r + Ode45.a41*Ode45.k1v + Ode45.a42*Ode45.k2v + Ode45.a43*Ode45.k3v), TempShip);
	Ode45.k5v = Ode45.h*f_nu((Ode45.t + Ode45.c5*Ode45.h), (TempShip.nu_r + Ode45.a51*Ode45.k1v + Ode45.a52*Ode45.k2v + Ode45.a53*Ode45.k3v + Ode45.a54*Ode45.k4v), TempShip);
	Ode45.k6v = Ode45.h*f_nu((Ode45.t + Ode45.c6*Ode45.h), (TempShip.nu_r + Ode45.a61*Ode45.k1v + Ode45.a62*Ode45.k2v + Ode45.a63*Ode45.k3v + Ode45.a64*Ode45.k4v + Ode45.a65*Ode45.k5v), TempShip);
	Ode45.k7v = Ode45.h*f_nu((Ode45.t + Ode45.c7*Ode45.h), (TempShip.nu_r + Ode45.a71*Ode45.k1v + Ode45.a73*Ode45.k3v + Ode45.a74*Ode45.k4v + Ode45.a75*Ode45.k5v + Ode45.a76*Ode45.k6v), TempShip);
	//zv = eta + b21*k1v + b22*k2v + b23*k3v + b24*k4v + b25*k5v + b26*k6v + b27*k7v;
	TempShip.nu_r = TempShip.nu_r + Ode45.b11*Ode45.k1v + Ode45.b12*Ode45.k2v + Ode45.b13*Ode45.k3v + Ode45.b14*Ode45.k4v + Ode45.b15*Ode45.k5v + Ode45.b16*Ode45.k6v + Ode45.b17*Ode45.k7v;
	return TempShip.nu_r;
}

Vector3d Calculate_Next_Eta(Solver Ode45, Ship TempShip) {
	Ode45.k1v = Ode45.h*f_eta(Ode45.t, TempShip.eta, TempShip);
	Ode45.k2v = Ode45.h*f_eta((Ode45.t + Ode45.c2*Ode45.h), (TempShip.eta + Ode45.a21*Ode45.k1v), TempShip);
	Ode45.k3v = Ode45.h*f_eta((Ode45.t + Ode45.c3*Ode45.h), (TempShip.eta + Ode45.a31*Ode45.k1v + Ode45.a32*Ode45.k2v), TempShip);
	Ode45.k4v = Ode45.h*f_eta((Ode45.t + Ode45.c4*Ode45.h), (TempShip.eta + Ode45.a41*Ode45.k1v + Ode45.a42*Ode45.k2v + Ode45.a43*Ode45.k3v), TempShip);
	Ode45.k5v = Ode45.h*f_eta((Ode45.t + Ode45.c5*Ode45.h), (TempShip.eta + Ode45.a51*Ode45.k1v + Ode45.a52*Ode45.k2v + Ode45.a53*Ode45.k3v + Ode45.a54*Ode45.k4v), TempShip);
	Ode45.k6v = Ode45.h*f_eta((Ode45.t + Ode45.c6*Ode45.h), (TempShip.eta + Ode45.a61*Ode45.k1v + Ode45.a62*Ode45.k2v + Ode45.a63*Ode45.k3v + Ode45.a64*Ode45.k4v + Ode45.a65*Ode45.k5v), TempShip);
	Ode45.k7v = Ode45.h*f_eta((Ode45.t + Ode45.c7*Ode45.h), (TempShip.eta + Ode45.a71*Ode45.k1v + Ode45.a73*Ode45.k3v + Ode45.a74*Ode45.k4v + Ode45.a75*Ode45.k5v + Ode45.a76*Ode45.k6v), TempShip);
	//zv = eta + b21*k1v + b22*k2v + b23*k3v + b24*k4v + b25*k5v + b26*k6v + b27*k7v;
	TempShip.eta = TempShip.eta + Ode45.b11*Ode45.k1v + Ode45.b12*Ode45.k2v + Ode45.b13*Ode45.k3v + Ode45.b14*Ode45.k4v + Ode45.b15*Ode45.k5v + Ode45.b16*Ode45.k6v + Ode45.b17*Ode45.k7v;
	return TempShip.eta;
}

double f(double t, double y) {
	//Random test function, y' = f(t, y) = -2y+1
	double res = -2 * y + 1;
	//std::cout << res << std::endl;
	return res;
}

Vector3d f_eta(double t, Vector3d eta, Ship CyberShipE1) {

	//eta' = R*nu
	Matrix3d R;
	double psi = eta(2);
	R << cos(psi), -sin(psi), 0,
		sin(psi), cos(psi), 0,
		0, 0, 1;
	return R*CyberShipE1.nu;
}

Vector3d f_nu(double t, Vector3d nu_r, Ship CyberShipE1) {
	//nu' = M_inv*(tau+tau_wind+tau_wave-C(nu)nu-D(nu)nu)
	CyberShipE1 = Get_Nonlinear_Matrices(CyberShipE1);
	//std::cout << "C: " << CyberShipE1.C << "\n" << "D: " << CyberShipE1.D << "\n\n" << std::endl;
	Vector3d res = CyberShipE1.M_inv*(CyberShipE1.tau_control - CyberShipE1.D*nu_r -CyberShipE1.C*nu_r);
	return res;
}

Ship Get_Nonlinear_Matrices(Ship TempShip) {

	TempShip.C << 0, -TempShip.m * TempShip.nu_r(2), TempShip.Y_v_dot * TempShip.nu_r(1) + (-TempShip.m * TempShip.x_g+TempShip.Y_r_dot)*TempShip.nu_r(2),
		TempShip.m*TempShip.nu_r(2), 0, -TempShip.X_u_dot * TempShip.nu_r(0),
		(TempShip.m * TempShip.x_g - TempShip.Y_r_dot)*TempShip.nu_r(2)-TempShip.Y_v_dot*TempShip.nu_r(1), TempShip.X_u_dot * TempShip.nu_r(0), 0;

	TempShip.D << -TempShip.X_uu*abs(TempShip.nu_r(0))-TempShip.X_u-TempShip.X_uuu*TempShip.nu_r(0)*TempShip.nu_r(0), 0, 0,
		0, -TempShip.Y_v-TempShip.Y_vv*abs(TempShip.nu_r(1))-TempShip.Y_vvv*TempShip.nu_r(1)*TempShip.nu_r(1) - TempShip.Y_rv*abs(TempShip.nu_r(2)),-TempShip.Y_r -TempShip.Y_vr*abs(TempShip.nu_r(1)) - TempShip.Y_rr*abs(TempShip.nu_r(2))-TempShip.Y_rrr*TempShip.nu_r(2)*TempShip.nu_r(2),
		0, -TempShip.N_v-TempShip.N_vv*abs(TempShip.nu_r(1)) - TempShip.N_rv*abs(TempShip.nu_r(2))-TempShip.N_vvv*TempShip.nu_r(1)*TempShip.nu_r(1), -TempShip.N_r-TempShip.N_vr*abs(TempShip.nu_r(1)) - TempShip.N_rr*abs(TempShip.nu_r(2))-TempShip.N_rrr*TempShip.nu_r(2)*TempShip.nu_r(2);
	return TempShip;
}


