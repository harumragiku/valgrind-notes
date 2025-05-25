#include <iostream>
#include <vector>
#include <cmath> // For std::sqrt, std::pow
#include <chrono> // For timing, not directly profiled by callgrind but good for example
#include <thread> // For std::this_thread::sleep_for

// Function that consumes a significant amount of CPU time
long long intensive_computation(int iterations) {
    long long sum = 0;
    for (int i = 0; i < iterations; ++i) {
        for (int j = 1; j < 2000; ++j) {
            sum += static_cast<long long>(std::sqrt(static_cast<double>(j) * i) + std::pow(1.0001, j));
        }
    }
    // Introduce a small artificial delay to ensure runtime if calculations are too fast
    // Note: sleep itself won't show as CPU work in callgrind but makes overall runtime longer
    if (iterations < 10) std::this_thread::sleep_for(std::chrono::milliseconds(50 * iterations));
    return sum;
}

// A moderately expensive function called multiple times
double moderate_work(int base, int repetitions) {
    double result = 0.0;
    for (int i = 0; i < repetitions; ++i) {
        result += std::sin(static_cast<double>(base + i)) * std::cos(static_cast<double>(base - i));
        for (int k = 0; k < 100; ++k) {
            result += std::log(static_cast<double>(k + base + 1));
        }
    }
    return result;
}

// A less expensive function, also called multiple times
void simple_task(int id) {
    // Simulate some light work
    double val = static_cast<double>(id);
    val = std::sqrt(val + 1.0);
    if (id % 100 == 0) { // Only print occasionally to reduce I/O overhead
      // std::cout << "Simple task " << id << " processed, result: " << val << std::endl;
    }
}

// Function to orchestrate calls
void run_simulation() {
    std::cout << "Starting intensive computation..." << std::endl;
    long long intensive_result = intensive_computation(200); // Reduced iterations for faster example
    std::cout << "Intensive computation finished. Result: " << intensive_result << std::endl;

    std::cout << "Starting moderate work series..." << std::endl;
    for (int i = 0; i < 50; ++i) {
        double mod_res = moderate_work(i * 10, 100); // Reduced repetitions
        if (i % 10 == 0) {
            // std::cout << "Moderate work batch " << i << " result: " << mod_res << std::endl;
        }
    }
    std::cout << "Moderate work finished." << std::endl;

    std::cout << "Starting simple tasks..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        simple_task(i);
    }
    std::cout << "Simple tasks finished." << std::endl;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    run_simulation();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Total execution time: " << duration.count() << " ms" << std::endl;

    return 0;
}

