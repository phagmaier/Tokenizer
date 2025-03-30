# 🔡 Tokenizer

[![License](https://img.shields.io/github/license/phagmaier/Tokenizer)](LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/phagmaier/Tokenizer?style=social)](https://github.com/phagmaier/Tokenizer/stargazers)
[![Last Commit](https://img.shields.io/github/last-commit/phagmaier/Tokenizer)](https://github.com/phagmaier/Tokenizer/commits/main)
[![Lines of Code](https://img.shields.io/tokei/lines/github/phagmaier/Tokenizer)](https://github.com/phagmaier/Tokenizer)
[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Repo Size](https://img.shields.io/github/repo-size/phagmaier/Tokenizer)](https://github.com/phagmaier/Tokenizer)

A high-performance tokenizer written in C, built for **blazing-fast** text processing. It uses **multithreading** and a **custom memory pool** to tokenize large files with minimal overhead. Tokenization is performed using the **Byte Pair Encoding (BPE)** algorithm.

---

## ⚠️ Current Status

- ✅ Vocabulary generation works  
- ⚠️ Functions for reading vocab and tokenizing new text exist but are **not yet tested**
- ⚠️ If data is sparse or repetitive in some areas or if token size is too large for text the program may run forever **NEED TO FIX THIS**

---

## ✨ Features

- 🔀 **Multithreaded processing** — Parallel tokenization using POSIX threads  
- 🧠 **Custom memory pool** — Fast and controlled memory allocation  
- 🧩 **Byte Pair Encoding (BPE)** — Efficient subword segmentation  
- 🧮 **Configurable vocabulary size**  
- 📦 **Adjustable chunk size per thread**

---

## 🛠️ Tech Stack

- **C** — Core implementation  
- **CMake** — Build system  
- **pthreads** — Concurrency

---

## 📋 Requirements

- 🧰 C compiler (GCC/Clang) with **C23 support**  
- ⚙️ CMake ≥ 3.16  
- 🛠️ Make (usually bundled with Unix-like systems)

---

## ⚙️ Build Instructions

```bash
# Clone the repository
git clone https://github.com/phagmaier/Tokenizer.git

# Enter the project directory
cd Tokenizer

# Create and move into the build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .
```

For a release build:

```bash
cmake --build . --config Release
```

✅ `runme` will be available in the `build` directory.

---

## 🚀 Usage

### CLI Flags

| Flag | Description           | Default              |
|------|-----------------------|----------------------|
| `-i` | Input file path       | `../data/data.txt`   |
| `-o` | Output file path      | `../data/myTokens.txt` |
| `-v` | Vocabulary size       | `10000`              |
| `-b` | Bytes per thread      | `250000`             |
| `-t` | Max threads           | `8`                  |

**Output Format:**

```
<token_id>,<token_string>\n
```

### Example

Custom run:

```bash
./runme -i ../data/infiniteJest.txt -v 10000 -b 500000 -o ../data/myTokens.txt -t 8
```

Default run:

```bash
./runme
```

⚠️ File paths should be relative to `build/` or be absolute.

---

## 🗂️ Project Structure

```
Tokenizer/
├── src/
│   ├── Dics.c/.h         # Dictionary & hash table
│   ├── Helper.c/.h       # Utility helpers
│   ├── Token.c/.h        # Token data structures
│   └── Tokenizer.c/.h    # Core tokenizer logic
├── CMakeLists.txt        # Build config
└── data/                 # Sample input/output files
```

---

## ⚡ Benchmarks

- 📘 *Infinite Jest* (3.2MB) — ~11 sec with 8 threads (default settings)  
- 📦 167MB corpus — ~15 sec with 15 threads, 50kB chunks

💡 Increase chunk size for better accuracy. Decrease it for more speed.

---

## 🧠 Limitations

1. ❌ Only supports ASCII  
2. ✂️ Chunk splitting may occur mid-word (needs smarter segmentation)

---

## ✅ TODO

- [ ] Prevent chunk splits mid-word  
- [ ] Test vocab-based tokenization  
- [ ] (Nice-to-have) Add NN/Transformer training integration
- [ ] Find a nice way to exit early and or try other chunk sections if chunk cannot generate more vocab

---

## 🤝 Contributing

No formal contributing guide yet — but PRs are welcome!

---

## 📫 Contact

**Parker Hagmaier**  
📧 [parkerhagmaier@gmail.com](mailto:parkerhagmaier@gmail.com)

```

