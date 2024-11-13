#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <chrono>
#include <random>
#include <limits>
#include "program1.cpp"
#include "program2.cpp"



int returnOne(){
    return 1;
}




using namespace std;

int main() {
    // Seed for random number generation
    std::random_device rd;
    std::mt19937 gen(rd());

    // Output file to write data
    std::ofstream outfile("runtime_data.csv");
    outfile << "n,dataset_index,program1_time,program2_time\n";

    // Input sizes
    std::vector<int> n_values = {1,10,20, 30, 40, 50,100,200,300,400,500, 1'000, 2'000, 3'000, 4'000, 5'000,6'000,7'000,8'000,9'000,10'000,};

    for (int n : n_values) {
        for (int dataset_index = 1; dataset_index <= 50000; ++dataset_index) {
            // Generate random W
            std::uniform_int_distribution<> W_distrib(1, 10000);
            int W = W_distrib(gen);

            // Generate random heights and widths
            std::vector<int> heights(n);
            std::vector<int> widths(n);

            // Generate heights (non-increasing)
            std::uniform_int_distribution<> height_distrib(1, 10000);
            heights[0] = height_distrib(gen);
            for (int i = 1; i < n; ++i) {
                std::uniform_int_distribution<> h_distrib(0, heights[i - 1]);
                heights[i] = h_distrib(gen);
            }

            // Generate widths
            std::uniform_int_distribution<> width_distrib(1, W);
            for (int i = 0; i < n; ++i) {
                widths[i] = width_distrib(gen);
            }

            // Measure runtime of program1
            auto start1 = std::chrono::high_resolution_clock::now();
            auto result1 = program1(n, W, heights, widths);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed1 = end1 - start1;

            // Measure runtime of program2
            auto start2 = std::chrono::high_resolution_clock::now();
            auto result2 = program2(n, W, heights, widths);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed2 = end2 - start2;

            // Output data
            outfile << n << "," << dataset_index << "," << elapsed1.count() << "," << elapsed2.count() << "\n";
        }
        std::cout << "Completed n = " << n << "\n";
    }
    outfile.close();
    return 0;
}
