# 🚀 Tokenizer: A Fast C Tokenizer

[![Build](https://img.shields.io/github/actions/workflow/status/phagmaier/Tokenizer/build.yml?branch=main)](https://github.com/phagmaier/Tokenizer/actions)
[![License](https://img.shields.io/github/license/phagmaier/Tokenizer)](./LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/phagmaier/Tokenizer?style=social)](https://github.com/phagmaier/Tokenizer/stargazers)
[![Last Commit](https://img.shields.io/github/last-commit/phagmaier/Tokenizer)](https://github.com/phagmaier/Tokenizer/commits/main)
[![Lines of Code](https://img.shields.io/tokei/lines/github/phagmaier/Tokenizer)](https://github.com/phagmaier/Tokenizer)

A high-performance tokenizer written in C, built for **blazing-fast** text processing. It uses **multithreading** and a **custom memory pool** to tokenize large files with minimal overhead. Tokenization is performed using the **Byte Pair Encoding (BPE)** algorithm.

---

## ✨ Key Features

- 🔀 **Multithreaded processing** — Efficient tokenization using multiple threads  
- 🧠 **Custom memory pool** — Fast memory management  
- 🧩 **Byte Pair Encoding (BPE)** — Subword tokenization strategy  
- 🧮 **Configurable vocabulary size**  
- 📦 **Adjustable chunk size per thread**

---

## 🛠️ Technologies Used

- **C** — Core implementation  
- **CMake** — Project build system  
- **POSIX Threads (pthreads)** — Concurrency support

---

## 📋 Prerequisites

- 🧰 C Compiler (GCC/Clang) — Supporting **C23 standard**
- 🧱 CMake ≥ 3.16  
- 🛠️ Make — Typically included in most Unix-like systems

---

## ⚙️ Installation

```bash
# Clone the repo
git clone https://github.com/phagmaier/Tokenizer.git

# Enter project directory
cd Tokenizer

# Create and enter build directory
mkdir build && cd build

# Configure project
cmake ..

# Build it
cmake --build .
```

✅ The `runme` executable will be available in the `build` directory.  
For a release build:  

```bash
cmake --build . --config Release
```

---

## 🚦 Usage & Flags

The tokenizer supports the following CLI flags:

| Flag | Description | Default |
|------|-------------|---------|
| `-i` | Input file path | `../data/data.txt` |
| `-o` | Output file path | `../data/myTokens.txt` |
| `-v` | Vocabulary size | `10000` |
| `-b` | Bytes per thread | `250000` |
| `-t` | Max number of threads | `8` |

**Output format**:  

```
<token_id>, <token_string>\n
```

---

## 🧪 Usage Examples

With custom flags:

```bash
./runme -i ../data/infiniteJest.txt -v 10000 -b 500000 -o ../data/myTokens.txt -t 8
```

Default run:

```bash
./runme
```

⚠️ **Note:** File paths should be relative to the `build` directory or be absolute paths.

---

## 🗂️ Project Structure

```
runme/
├── src/
│   ├── Dics.c / .h         # Dictionary implementation
│   ├── Helper.c / .h       # Utility functions
│   ├── Token.c / .h        # Token logic
│   └── Tokenizer.c / .h    # Core tokenizer
├── CMakeLists.txt          # Build config
└── data/                   # Input/output files
```

---

## ⚡ Performance Benchmarks

- **Large file (167MB)** — ~15 seconds using **15 threads**, chunk size ~50k bytes  
- **Infinite Jest (3.2MB)** — ~11 seconds with **8 threads**, default settings  

ℹ️ For **better accuracy**, use a larger chunk size per thread. For **max performance**, reduce chunk size.

---

## 🧠 Known Limitations

1. ❌ Only supports ASCII
2. ✂️ New chunks may start mid-word — need better segmentation

---

## 🤝 Contributing

Currently, this project does not have explicit contribution guidelines. PRs are still welcome!

---

## 📫 Contact

**Parker Hagmaier**  
📧 [parkerhagmaier@gmail.com](mailto:parkerhagmaier@gmail.com)

---
