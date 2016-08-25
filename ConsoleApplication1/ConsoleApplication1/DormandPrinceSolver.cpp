#include "stdafx.h"
#include "Ode45.h"
#include <math.h>


void Ode45(double tf) {
	long double epsilon = 0.000001;
	long double t = 0L;
	long double y = 0L;
	long double h = 0.001L;
	long double h_min = 0.00001L;
	long double h_max = 0.01L;
	double a21, a31, a32, a41, a42, a43, a51, a52, a53, a54, a61, a62, a63, a64, a65, a71, a72, a73, a74, a75, a76, c2, c3, c4, c5, c6, c7, b11, b12, b13, b14, b15, b16, b17, b21, b22, b23, b24, b25, b26, b27;

	a21 = 1.0L / 5L;
	a31 = 3.0L / 40;
	a32 = 9.0L / 40;
	a41 = 44.0L / 45;
	a42 = -56.0L / 15;
	a43 = 32.0L / 9;
	a51 = 19372.0L / 6561;
	a52 = -25360.0L / 2187;
	a53 = 64448.0L / 6561;
	a54 = -212.0L / 729;
	a61 = 9017.0L / 3168;
	a62 = -355.0L / 33;
	a63 = 46732.0L / 5247;
	a64 = 49.0L / 176;
	a65 = -5103.0L / 18656;
	a71 = 35.0L / 384;
	a72 = 0;
	a73 = 500.0L / 1113;
	a74 = 125.0L / 192;
	a75 = -2187.0L / 6784;
	a76 = 11.0L / 84;
	c2 = 1.0L / 5;
	c3 = 3.0L / 10;
	c4 = 4.0L / 5;
	c5 = 8.0L / 9;
	c6 = 1.0L;
	c7 = 1.0L;
	b11 = 35.0L / 384;
	b12 = 0.0L;
	b13 = 500.0L / 1113;
	b14 = 125.0L / 192;
	b15 = -2187.0L / 6784;
	b16 = 11.0L / 84;
	b17 = 0.0L;
	b21 = 5179.0L / 57600;
	b22 = 0.0L;
	b23 = 7571.0L / 16695;
	b24 = 393.0L / 640;
	b25 = -92097.0L / 339200;
	b26 = 187.0L / 2100;
	b27 = 1.0L / 40;
	long double k1, k2, k3, k4, k5, k6, k7, z, s, err;
	std::ofstream resWrite;
	resWrite.open("res.txt");


	while (t <= tf) {
		resWrite << y << " " << t << "\n";
		k1 = h*f(t, y);
		k2 = h*f((t + c2*h), double(y + a21*k1));
		//std::cout << k1 << ", " << a21 << ", " << double(y + a21*k1) << std::endl;
		k3 = h*f((t + c3*h), (y + a31*k1 + a32*k2));
		k4 = h*f((t + c4*h), (y + a41*k1 + a42*k2 + a43*k3));
		k5 = h*f((t + c5*h), (y + a51*k1 + a52*k2 + a53*k3 + a54*k4));
		k6 = h*f((t + c6*h), (y + a61*k1 + a62*k2 + a63*k3 + a64*k4 + a65*k5));
		k7 = h*f((t + c7*h), (y + a71*k1 + a73*k3 + a74*k4 + a75*k5 + a76*k6));
		z = y + b21*k1 + b22*k2 + b23*k3 + b24*k4 + b25*k5 + b26*k6 + b27*k7;
		y = y + b11*k1 + b12*k2 + b13*k3 + b14*k4 + b15*k5 + b16*k6 + b17*k7;
		err = abs(z - y);
		//std::cout << b11*k1 + b12*k2 + b13*k3 + b14*k4 + b15*k5 + b16*k6 + b17*k7 << std::endl;
		s = pow((epsilon*h) / (2 * err), 1 / 5);
		h = s*h;
		if (h < h_min)
			h = h_min;
		if (h > h_max)
			h = h_max;
		t = t + h;
	}
}

//double abs(double input) {
//	if (input > 0) {
//		return input;
//	}
//	else {
//		return (-1 * input);
//	}
//}

double f(double t, double y) {
	//Random test function, y' = f(t, y) = -2y+1
	double res = -2 * y + 1;
	//std::cout << res << std::endl;
	return res;
}


