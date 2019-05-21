#include <vector>
#include <parallel/algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>
#include <string>
using namespace std;
using namespace std::chrono;

vector<int> reversed_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = size - i;
    }
    return v;
}

vector<int> sorted_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = i;
    }
    return v;
}

vector<int> random_vector(int size) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 rng(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dist(1, size);

    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = dist(rng);
    }
    return v;
}


double bench_omp_sort(int size, vector<int> (*generator)(int), int iterations, int thread_pool_size) {
    double avg = 0;

    // Setting the number of threads
    omp_set_dynamic(0);
    omp_set_num_threads(thread_pool_size);

    for (int i = 0; i < iterations; i++) {
        vector<int> v = generator(size);

        high_resolution_clock::time_point begin_time = high_resolution_clock::now();

#pragma omp single
{
        __gnu_parallel::stable_sort(v.begin(), v.end());
}

        high_resolution_clock::time_point finish_time = high_resolution_clock::now();

        avg += duration_cast<nanoseconds>(finish_time - begin_time).count();
    }

    return avg / iterations;
}

void bench_omp(vector<int> sizes, vector<int> thread_pool_sizes, vector<int> (*generator)(int), int iterations, string filename) {
    ofstream data_file;
    data_file.open(filename);
    data_file << "# Size Threads OMP_sort\n";

    double duration_tmp;
    for (auto size = sizes.begin(); size != sizes.end(); ++size) {
        for (auto thread_pool_size = thread_pool_sizes.begin(); thread_pool_size != thread_pool_sizes.end(); ++thread_pool_size) {
            duration_tmp = bench_omp_sort(*size, generator, iterations, *thread_pool_size);
            data_file << *size << " " << *thread_pool_size << " " << to_string(round(duration_tmp)) << endl;
        }
    }
    data_file.close();
}


int main() {
    int iterations = 10;
    vector<int> sizes {100000000};
    vector<int> thread_pool_sizes {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    				   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

    cout << "Random\n";
    bench_omp(sizes, thread_pool_sizes, random_vector, iterations, "data/omp_random.dat");
    cout << "Reversed\n";
    bench_omp(sizes, thread_pool_sizes, reversed_vector, iterations, "data/omp_reversed.dat");
    cout << "Sorted\n";
    bench_omp(sizes, thread_pool_sizes, sorted_vector, iterations, "data/omp_sorted.dat");

    return 0;
}
