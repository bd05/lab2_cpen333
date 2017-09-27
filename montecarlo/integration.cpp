#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

// three-dimensional point
struct Point {
	double x, y, z;
};

// virtual base class for functions
class Function {
public:
	virtual double eval(double x, double y, double z) = 0;
};

// computes x*fn(x,y,z)
class XFunction : public Function {
	Function& fn;
public:
	XFunction(Function& fn) : fn(fn){};
	double eval(double x, double y, double z) {
		return x*fn.eval(x, y, z);
	}
};

// computes y*fn(x,y,z)
class YFunction : public Function {
	Function& fn;
public:
	YFunction(Function& fn) : fn(fn){};
	double eval(double x, double y, double z) {
		return y*fn.eval(x, y, z);
	}
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
	Function& fn;
public:
	ZFunction(Function& fn) : fn(fn){};
	double eval(double x, double y, double z) {
		return z*fn.eval(x, y, z);
	}
};

// new function for representing density 1
class Density1 : public Function {
public:
	double eval(double x, double y, double z) {
		double norm2 = x*x + y*y + z*z;
		if (norm2 > 1) {
			return 0;
		}
		return std::exp(-norm2);
	}
};

// new function for representing density 2
class Density2 : public Function {
public:
	double eval(double x, double y, double z) {
		return std::abs(x) + std::abs(y) + std::abs(z);
	}
};

// new function for representing density 3
class Density3 : public Function {
public:
	double eval(double x, double y, double z) {
		return pow(x - 1, 2) + pow(y - 2, 2) + pow(z - 3, 2);
	}
};

//constant density to check
// new function for representing density test
class DensityTest : public Function {
public:
	double eval(double x, double y, double z) {
		return 0.5;
	}
};

int main() {
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);
	double x, y, z;
	double pIntegral = 0;
	double cx = 0;
	double cy = 0;
	double cz = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	Density3 d1;
	XFunction xd1(d1);  // x*d1(x,y,z)
	YFunction yd1(d1);  // y*d1(x,y,z)
	ZFunction zd1(d1);  // z*d1(x,y,z)
	int posCount = 0;
	int numSamples = 1000000;

	while (posCount < numSamples){
		x = dist(rnd);
		y = dist(rnd);
		z = dist(rnd);

		if (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)) <= 1){
			pIntegral += d1.eval(x, y, z);
			px += xd1.eval(x, y, z);
			py += yd1.eval(x, y, z);
			pz += zd1.eval(x, y, z);
			posCount++;
		}
	}

	cx = px / pIntegral;
	cy = py / pIntegral;
	cz = pz / pIntegral;

	std::cout << "cx: " << cx << std::endl;
	std::cout << "cy: " << cy << std::endl;
	std::cout << "cz: " << cz << std::endl;


	/*std::cout << "d1.eval(0.1,0.2,0.3): " << d1.eval(0.1, 0.2, 0.3) << std::endl;
	std::cout << "xd1.eval(0.1,0.2,0.3): " << xd1.eval(0.1, 0.2, 0.3) << std::endl;
	std::cout << "yd1.eval(0.1,0.2,0.3): " << yd1.eval(0.1, 0.2, 0.3) << std::endl;
	std::cout << "zd1.eval(0.1,0.2,0.3): " << zd1.eval(0.1, 0.2, 0.3) << std::endl;*/

	return 0;
}