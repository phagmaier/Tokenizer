# Tokenizer: A Fast C Tokenizer

This project implements a high-performance tokenizer written in C, designed for efficient text processing. It utilizes multithreading and a custom memory pool for optimized speed and memory management.  The tokenizer employs a Byte Pair Encoding (BPE) algorithm to generate a vocabulary of tokens.

## NOTE ON CURRENT STATE OF THE PROJECT

1. The current implementation does not limit by word it just always merges the largest pairs
   - changing this in the newest update to limit by word
2. This version was just to test out the algorithm newest version (coming soon) will do it the proper way 
3. Still need to add a way to then tokenize new test given the current tokens 
4. Need to add all individual characters 
5. For this and all future implementations of this we will limit to ASCII characters 

## Key Features

* **Multithreaded processing:**  Leverages multiple threads to significantly speed up tokenization of large files.
* **Custom memory pool:** Reduces memory allocation overhead for improved performance.
* **Byte Pair Encoding (BPE):**  Employs a BPE algorithm for subword tokenization.
* **Configurable vocabulary size:** Allows you to specify the desired size of the output vocabulary.
* **Customizable chunk size:** Controls the size of data processed by each thread.

## Technologies Used

* **C:** The core language of the project.
* **CMake:** Used for building and managing the project.
* **POSIX Threads (pthreads):** For multithreading support.

## Prerequisites

* **C Compiler (e.g., GCC, Clang):**  A C compiler supporting C23 standard is required.
* **CMake:**  Version 3.16 or higher.
* **Make:**  A build system (typically included with most Unix-like systems).

## Installation

1. **Clone the repository:**
   
   ```bash
   git clone https://github.com/phagmaier/Tokenizer.git
   ```

2. **Navigate to the project directory:**
   
   ```bash
   cd runme
   ```

3. **Create a build directory:**
   
   ```bash
   mkdir build
   cd build
   ```

4. **Configure the project:**
   
   ```bash
   cmake ..
   ```

5. **Build the project:**
   
   ```bash
   cmake --build .
   ```
   
   This will create the executable `runme` in the `build` directory.  For a release build use `cmake --build . --config Release`.

## FLAGS

The tokenizer takes the following arguments:  

* -i: Input file path (default: ../data/data.txt).
* -o: Output file path (default: ../data/myTokens.txt).
* -v: Vocabulary size (default: 20000).
* -b: Bytes per thread (default: 500000).  
* -t: Maximum number of threads (default: 8).
* The output format is: `<token_id>, <token_string>\n`.

## Usage Examples

To run the tokenizer with flags:

```bash
./runme -i ../data/infiniteJest.txt -v 10000 -b 500000 -o ../data/myTokens.txt -t 8
```

Or without

```bash
./runme 
```

**Important:** The you use or pass in must be relative to the `build` directory or provide the correct absolute path.

## Project Structure

```
runme/
├── src/
│   ├── Dics.c       // Implementation of dictionary functions.
│   ├── Dics.h       // Header file for dictionary functions.
│   ├── Helper.c     // Implementation of helper functions.
│   ├── Helper.h     // Header file for helper functions.
│   ├── Token.c      // Implementation of token functions.
│   ├── Token.h      // Header file for token functions.
│   ├── Tokenizer.c  // Implementation of the tokenizer.
│   └── Tokenizer.h  // Header file for the tokenizer.
├── CMakeLists.txt  // CMake build configuration file.
└── data/           //Directory where input text and output file resides.
```

## PERFORMANCE

With 15 threads on a framework laptop that uses the AMD Ryzen 7040 Series CPUs
this program was able to process ~3k screenplays \(~438071393 bytes\) and generate
50k unique vocab words in ~20 minutes

## Contributing

This project does not have explicit contribution guidelines.

## Contact

Parker Hagmaier: <parkerhagmaier@gmail.com>
