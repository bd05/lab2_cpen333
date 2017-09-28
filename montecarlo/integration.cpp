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

// count number of hits using nsamples, populates hits[idx]
void circle_hits(std::vector<double>& hits, std::vector<double>& hits_x, std::vector<double>& hits_y, std::vector<double>& hits_z, int idx, int nsamples) {

	// single instance of random engine and distribution
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);
	double x;
	double y;
	double z;
	double distanceFromOrigin;
	Density3 d1;
	XFunction xd1(d1);  // x*d1(x,y,z)
	YFunction yd1(d1);  // y*d1(x,y,z)
	ZFunction zd1(d1);  // z*d1(x,y,z)

	for (int i = 0; i < nsamples; ++i) {
		x = dist(rnd);
		y = dist(rnd);
		z = dist(rnd);
		distanceFromOrigin = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		if (distanceFromOrigin <= 1){
			hits[idx] += d1.eval(x, y, z);
			hits_x[idx] += xd1.eval(x, y, z);
			hits_y[idx] += yd1.eval(x, y, z);
			hits_z[idx] += zd1.eval(x, y, z);
		}
	}
}


double estimate_p_multithread(int nsamples) {
	// number of available cores
	int nthreads = std::thread::hardware_concurrency();
	std::cout << "nthreads available: " << nthreads << std::endl;
	double p = 0;
	double px = 0;
	double py = 0;
	double pz = 0;
	double cx = 0;
	double cy = 0;
	double cz = 0;

	// hit counts
	std::vector<double> hits(nthreads, 0);
	std::vector<double> hits_x(nthreads, 0);
	std::vector<double> hits_y(nthreads, 0);
	std::vector<double> hits_z(nthreads, 0);

	// create and store threads
	std::vector<std::thread> threads;
	int msamples = 0; // samples accounted for
	for (int i = 0; i<nthreads - 1; ++i) {
		threads.push_back(
			std::thread(circle_hits, std::ref(hits), std::ref(hits_x), std::ref(hits_y), std::ref(hits_z), i, nsamples / nthreads));
		msamples += nsamples / nthreads;
	}
	// remaining samples
	threads.push_back(
		std::thread(circle_hits, std::ref(hits), std::ref(hits_x), std::ref(hits_y), std::ref(hits_z), nthreads - 1, nsamples - msamples));

	// wait for threads to finish
	for (int i = 0; i<nthreads; ++i) {
		threads[i].join();
	}

	// estimate centre of mass
	for (int i = 0; i < nthreads; ++i) {
		p += hits[i];
		px += hits_x[i];
		py += hits_y[i];
		pz += hits_z[i];
	}

	cx = px / p;
	cy = py / p;
	cz = pz / p;

	std::cout << "cx: " << cx << std::endl;
	std::cout << "cy: " << cy << std::endl;
	std::cout << "cz: " << cz << std::endl;

	return p;
}


int main() {
	/*static std::default_random_engine rnd;
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
			//turn the cumulative sums eval()'s into threads
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
	*/

	estimate_p_multithread(1000);
	estimate_p_multithread(10000);
	estimate_p_multithread(100000);
	estimate_p_multithread(1000000);

	return 0;
}