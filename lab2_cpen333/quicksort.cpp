#include <iostream>
#include <thread>
#include <vector>

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
	return;
}
// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
int partition(std::vector<int>& data, int low, int high) {
	// your code here
	int pivot = data[high];
	int partitionIndex = low;
	for (int i = low; i < high; i++){
		if (data[i] <= pivot){
			swap(&data[i], &data[partitionIndex]);
			partitionIndex++;
		}
	}
	swap(&data[partitionIndex], &data[high]);
	return partitionIndex;
}

// sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high) {
	// your code here
	if (low < high){
		int partitionIndex = partition(data,low, high);
		quicksort(data, low, partitionIndex - 1);
		quicksort(data, partitionIndex + 1, high);
	}
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high) {
	// your code here
}

int main() {

	// create two copies of random data
//	const int VECTOR_SIZE = 1000000;
	const int VECTOR_SIZE = 10;
	std::vector<int> v1(VECTOR_SIZE, 0);
	// fill with random integers
	std::cout << "before sorting: " << std::endl;
	for (int i = 0; i<VECTOR_SIZE; ++i) {
		v1[i] = rand();
		std::cout << v1[i] << " ";
	}

	std::vector<int> v2 = v1;  // copy all contents

	// sort v1 using sequential algorithm
	std::cout << "after sorting, v1: " << std::endl;
	quicksort(v1, 0, v1.size() - 1);
	for (int i = 0; i < VECTOR_SIZE; ++i){
		std::cout << v1[i] << ", ";
	}

	// sort v2 using parallel algorithm
	parallel_quicksort(v2, 0, v2.size() - 1);

	std::cout << std::endl << "end" << std::endl;
	return 0;
}