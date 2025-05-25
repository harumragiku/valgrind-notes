#include <iostream>
#include <vector>

void cause_leak() {
    // Intentionally allocate memory that is not freed.
    int* leaky_array = new int[10];
    leaky_array[0] = 1; // Just to use it briefly
    std::cout << "Leaky function called. Memory allocated at: " << leaky_array << std::endl;
    // Missing: delete[] leaky_array;
}

void use_uninitialized_memory() {
    int uninitialized_value;
    // Intentionally reading from uninitialized_value.
    // Note: Modern compilers might optimize this away or warn heavily.
    // To make it more likely to be caught by Valgrind, we'll use it in a condition.
    if (uninitialized_value > 0) {
        std::cout << "Uninitialized value is positive." << std::endl;
    } else {
        std::cout << "Uninitialized value is not positive (or garbage)." << std::endl;
    }

    // Another example: allocating memory but not initializing all of it
    int* partially_initialized_array = new int[5];
    partially_initialized_array[0] = 100;
    // partially_initialized_array[1] through [4] are uninitialized.

    std::cout << "Value at index 1: " << partially_initialized_array[1] << " (potentially uninitialized)" << std::endl;
    // This read above is what Valgrind should flag.

    delete[] partially_initialized_array; // Clean up this allocation
}

int main() {
    std::cout << "Starting memory_example program." << std::endl;

    cause_leak();
    use_uninitialized_memory();

    std::cout << "memory_example program finished." << std::endl;
    return 0;
}

