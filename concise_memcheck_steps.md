# Quick Guide: Using Valgrind `memcheck`

This guide provides the bare essentials to quickly find a memory leak using Valgrind's `memcheck` tool.

## 1. Create an Example C++ Program (`quick_memcheck_example.cpp`)

Create a file named `quick_memcheck_example.cpp` with the following content:

```cpp
// quick_memcheck_example.cpp
#include <iostream>

void memory_leak_function() {
    int* my_array = new int[10]; // Allocate memory
    my_array[0] = 1;             // Use the memory
    std::cout << "Allocated memory at: " << my_array << std::endl;
    // Oops! We forgot to deallocate with: delete[] my_array;
}

int main() {
    memory_leak_function();
    std::cout << "Program finished." << std::endl;
    return 0;
}
```
This program allocates an array of 10 integers but intentionally does not free this memory, causing a leak.

## 2. Compile with Debugging Symbols

Compile the program using `g++`, including the `-g` flag to add debugging symbols. This helps Valgrind provide more precise error locations.

```bash
g++ -g quick_memcheck_example.cpp -o quick_memcheck_example
```

## 3. Run with Valgrind `memcheck`

Execute the compiled program under Valgrind `memcheck`:

```bash
valgrind --leak-check=full ./quick_memcheck_example
```
*   `--leak-check=full`: This option tells `memcheck` to show detailed information about each leaked block.

## 4. What to Look For in the Output

After the program's normal output, Valgrind will print its analysis. Key sections to examine are:

*   **`HEAP SUMMARY`**:
    *   Look for `in use at exit: X bytes in Y blocks`. If X is greater than 0, it indicates memory was still allocated when the program ended.
    *   Example: `==XXXXX== HEAP SUMMARY:`
      `==XXXXX== in use at exit: 40 bytes in 1 blocks` (40 bytes = 10 ints * 4 bytes/int)

*   **`LEAK SUMMARY`**:
    *   Look for `definitely lost: X bytes in Y blocks`. "Definitely lost" means Valgrind is sure this memory is leaked.
    *   Example: `==XXXXX== LEAK SUMMARY:`
      `==XXXXX== definitely lost: 40 bytes in 1 blocks`

Valgrind will also provide a stack trace pointing to where the leaked memory was allocated (e.g., inside `memory_leak_function` in our example). This helps you locate the `new` call that lacks a corresponding `delete`.
```

