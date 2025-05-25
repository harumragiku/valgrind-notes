# Using Valgrind with the `dstogov/ir` JIT Framework

This document provides general guidance on how to use Valgrind's `memcheck` and `callgrind` tools with the `dstogov/ir` project.

## 1. Project Overview (`dstogov/ir`)

The `dstogov/ir` project, found on [GitHub](https://github.com/dstogov/ir), is a C project that implements a lightweight Just-In-Time (JIT) compilation framework.

Key characteristics relevant for Valgrind usage:
*   It is written in C.
*   It uses `make` for its build system.
*   It includes a test suite that can be run using `make test`.
*   It provides an `./ir` driver program to execute IR files (e.g., benchmarks like `bench/mandelbrit.ir`).

## 2. General Steps for Using Valgrind with `dstogov/ir`

Here’s a general approach to applying Valgrind to this project. Adjustments may be necessary based on the specifics of the Makefile and test execution.

### Step 1: Clone the Repository

First, obtain the source code:
```bash
git clone https://github.com/dstogov/ir.git
cd ir
```

### Step 2: Compile with Debug Symbols (`-g`)

For Valgrind to provide meaningful output (mapping errors and performance data to source code lines and function names), the project **must** be compiled with debugging symbols.

*   **Importance of `-g`:** The `-g` flag tells the C compiler (GCC or Clang) to include debug information in the compiled binaries. Without it, Valgrind's reports will be much harder to interpret, often showing only memory addresses.
*   **Check the `Makefile`:** Open the `Makefile` in the `ir` directory and look for the `CFLAGS` variable (or similar, like `CCFLAGS` or `COMPILE_FLAGS`).
*   **Adding `-g`:**
    *   If `-g` is not already present, you can try to prepend it when invoking `make`. For memory checking with `memcheck`, it's also often beneficial to reduce optimization levels to get clearer, more direct mappings from execution to source code.
        ```bash
        CFLAGS="-g -O0" make clean all
        ```
        (Use `make clean` or `make clean all` first to ensure all files are recompiled with the new flags. `-O0` disables optimization, `-O1` is also a good option for debugging.)
    *   Alternatively, you can edit the `Makefile` directly. Find the line defining `CFLAGS` (or the primary C compilation flags) and add `-g` (and optionally `-O0` or `-O1`) to it. For example:
        Original: `CFLAGS = -Wall -Wextra -Wno-unused-parameter -std=c99 -O2`
        Modified: `CFLAGS = -g -O0 -Wall -Wextra -Wno-unused-parameter -std=c99`
        Then, recompile with `make clean all` or `make`.

### Step 3: Using `memcheck` (Memory Error Detection)

`memcheck` helps find memory leaks, uses of uninitialized memory, invalid reads/writes, etc.

*   **On the test suite (`make test`):**
    If `make test` directly executes the compiled test programs, you might be able to prefix the `make` command with Valgrind:
    ```bash
    valgrind --leak-check=full --show-leak-kinds=all make test
    ```
    **Note:** This approach works best if `make test` itself runs the compiled binaries in a way that Valgrind can directly wrap. If `make test` involves complex shell scripts that invoke other scripts or manage processes in non-standard ways, this might not capture all relevant activity, or it might capture too much (e.g., the shell itself). You might need to identify the specific executables run by `make test` and run Valgrind on them individually.

*   **On the `./ir` driver (example with a benchmark file):**
    To check the `./ir` driver program when it runs a specific IR file (like a benchmark):
    ```bash
    valgrind --leak-check=full --show-leak-kinds=all ./ir bench/mandelbrit.ir --run
    ```
    *   `--leak-check=full`: Shows details for each distinct leak.
    *   `--show-leak-kinds=all`: Shows all leak types (definitely lost, indirectly lost, possibly lost, still reachable).

    **Interpreting Output:**
    Look for the **`HEAP SUMMARY`** section, specifically the `in use at exit` line. Non-zero values indicate memory was still allocated.
    Also, examine the **`LEAK SUMMARY`**, paying close attention to "definitely lost" and "indirectly lost" blocks. Valgrind will provide stack traces for these, indicating where the leaked memory was allocated.

### Step 4: Using `callgrind` (Performance Profiling)

`callgrind` measures instruction execution counts to help identify performance bottlenecks.

*   **On the `./ir` driver with a benchmark:**
    To profile the `./ir` driver:
    ```bash
    valgrind --tool=callgrind ./ir bench/mandelbrit.ir --run
    ```
    This command will run your program (much slower than usual) and, upon completion, will generate an output file named `callgrind.out.<pid>` (e.g., `callgrind.out.12345`) in the current directory.

### Step 5: Analyzing `callgrind` Output

The `callgrind.out.<pid>` file is not human-readable. Use a visualization tool:

*   **KCachegrind (Linux/KDE) or QCachegrind (Linux, macOS, Windows):**
    *   Install one of these tools (e.g., `sudo apt-get install kcachegrind` or `sudo apt-get install qcachegrind` on Debian/Ubuntu).
    *   Launch the tool and open the `callgrind.out.<pid>` file.
*   **What to look for:**
    *   **Flat Profile:** Sort this list by the "Self" column (descending) to see which functions consumed the most instructions (CPU time) directly.
    *   **Source Code View:** Select a function from the list to see its source code annotated with instruction counts per line, helping you pinpoint the exact lines that are most expensive within that function.
    *   **Callee/Caller Maps:** Explore these to understand call relationships and how costs propagate.

## 3. General Advice

*   The steps above are general guidelines. The specifics of the `dstogov/ir` build system (`Makefile`) or test execution might require adjustments. For instance, if tests are run via scripts, you might need to edit those scripts to prepend `valgrind` to the actual program invocation.
*   Valgrind runs programs significantly slower (20x-50x or more). Be patient, and for profiling, ensure the program runs long enough to capture representative behavior. For `memcheck`, even short runs that exercise specific functionality can be very useful.
*   For more detailed information on interpreting Valgrind's output, suppressing errors from known sources (like system libraries), or understanding advanced options, refer to comprehensive Valgrind documentation (such as the guides generated in previous steps or online resources).
*   Given that `dstogov/ir` is a JIT, profiling with `callgrind` will show the cost of both the JIT compiler itself and the code it generates. Interpreting this might require distinguishing between time spent in JIT compilation routines and time spent in the generated code. Advanced `callgrind` features or careful analysis of function names might be necessary. For `memcheck`, ensure that memory used by the JIT for its own data structures and for the generated code is properly managed.

This guide should provide a good starting point for applying Valgrind to the `dstogov/ir` project.
```
