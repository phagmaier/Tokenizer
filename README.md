# Runme: A Fast C Tokenizer

This project implements a high-performance tokenizer written in C, designed for efficient text processing. It utilizes multithreading and a custom memory pool for optimized speed and memory management.  The tokenizer employs a Byte Pair Encoding (BPE) algorithm to generate a vocabulary of tokens.

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
   git clone <repository_url>
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
* -v: Vocabulary size (default: 15000).
* -b: Bytes per thread (default: 500000).  
* -t: Maximum number of threads (default: 4).
* The output format is: `<token_id>, <token_string>\n`.

## Usage Examples

To run the tokenizer with flags:

```bash
./runme -i ../data/infiniteJest.txt -v 10000 -b 500000 -o ../data/myTokens.txt -t 8
```

Or without. Note this assumes you have a file called data.txt in the ../data directory

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

## Contributing

This project does not have explicit contribution guidelines.

## Contact

[Parker Hagmaier] - [parkerhagmaier@gmail.com]
