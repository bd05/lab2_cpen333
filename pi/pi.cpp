#include <thread>
#include <iostream>
#include <random>

// count number of hits using nsamples, populates hits[idx]
void pi_hits(std::vector<int>& hits, int idx, int nsamples) {
	std::cout << "idx starts at: " << idx << std::endl;
	// single instance of random engine and distribution
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);

	// YOUR CODE HERE
	double x;
	double y;
	double distanceFromOrigin;

	for (int i = 0; i< nsamples; ++i) {
		//std::cout << dist(rnd) << std::endl;
		x = dist(rnd);
		y = dist(rnd);
		distanceFromOrigin = sqrt(pow(x, 2) + pow(y, 2));
		if (distanceFromOrigin <= 1){
			hits[idx] += 1;
		}
	}
	/*std::cout << "hits in individual thread" << std::endl;
	for (int i = 0; i < hits.size(); ++i) {
		std::cout << "hits " << i << " " << hits[i] << std::endl;
	}*/
}

// divides work among threads intelligently
double estimate_pi_multithread(int nsamples) {

	// number of available cores
	int nthreads = std::thread::hardware_concurrency();

	// hit counts
	std::vector<int> hits(nthreads, 0);

	// create and store threads
	std::vector<std::thread> threads;
	int msamples = 0; // samples accounted for
	for (int i = 0; i<nthreads - 1; ++i) {
		threads.push_back(
			std::thread(pi_hits, std::ref(hits), i, nsamples / nthreads));
		msamples += nsamples / nthreads;
	}
	// remaining samples
	threads.push_back(
		std::thread(pi_hits, std::ref(hits), nthreads - 1, nsamples - msamples));

	// wait for threads to finish
	for (int i = 0; i<nthreads; ++i) {
		threads[i].join();
	}

	// estimate pi
	double pi = 0;
	for (int i = 0; i<nthreads; ++i) {
		std::cout << "hits " << i << " " << hits[i] << std::endl;
		pi += hits[i];
	}
	pi = pi / nsamples * 4;

	return pi;
}

// generates a random sample and sets the value of `inside`
// to true if within the unit circle
void pi_sampler(std::vector<bool>& hits, int idx) {

	// single instance of random engine and distribution
	static std::default_random_engine rnd;
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);

	// YOUR CODE HERE
	double x;
	double y;
	double distanceFromOrigin;

		//std::cout << dist(rnd) << std::endl;
	x = dist(rnd);
	y = dist(rnd);
	distanceFromOrigin = sqrt(pow(x, 2) + pow(y, 2));
	if (distanceFromOrigin <= 1){
		hits[idx] = true;
	}

}

// naively uses multithreading to try to speed up computations
double estimate_pi_multithread_naive(int nsamples) {
	// stores whether each sample falls within circle
	std::vector<bool> hits(nsamples, false);

	// create and store all threads
	std::vector<std::thread> threads;
	for (int i = 0; i<nsamples; ++i) {
		threads.push_back(std::thread(pi_sampler, std::ref(hits), i));
	}

	// wait for all threads to complete
	for (int i = 0; i<nsamples; ++i) {
		threads[i].join();
	}

	// estimate our value of PI
	double pi = 0;
	for (int i = 0; i<nsamples; ++i) {
		if (hits[i]) {
			pi = pi + 1;
		}
	}
	pi = pi / nsamples * 4;

	return pi;
}

double estimate_pi(int nsamples) {
	// your code here
	// specify the engine and distribution.
	std::default_random_engine rnd(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> dist(-1.0, 1.0);

	double x;
	double y;
	double distanceFromOrigin;
	double hits = 0;

	for (int i = 0; i< nsamples; ++i) {
		//std::cout << dist(rnd) << std::endl;
		x = dist(rnd);
		y = dist(rnd);
		distanceFromOrigin = sqrt(pow(x, 2) + pow(y, 2));
		if (distanceFromOrigin <= 1){
			hits++;
		}
	}

	double valPi = (hits / (double) nsamples) * 4;

	return valPi;
}

int main() {

	double pi = estimate_pi(1000);
	std::cout << "My estimate of PI is: " << pi << std::endl;

	double pi2 = estimate_pi_multithread_naive(1000);
	std::cout << "My second estimate of PI is: " << pi << std::endl;

	double pi3 = estimate_pi_multithread(100);
	std::cout << "My third estimate of PI is: " << pi << std::endl;

	return 0;
}