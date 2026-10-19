> **Earlier iteration.** Superseded by [Zokenizer](https://github.com/phagmaier/Zokenizer) (Zig) and the BPE tokenizer built into [zlm](https://github.com/phagmaier/zlm).

# 🔡 Tokenizer

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

A high-performance, multithreaded tokenizer written in **C**, designed for fast and scalable text preprocessing using the **Byte Pair Encoding (BPE)** algorithm. It uses a custom memory pool and lock-free optimizations for maximum efficiency.

---

## 🧠 What Is BPE?

**Byte Pair Encoding (BPE)** is a tokenization algorithm that:

1. Starts by splitting text into individual characters.
2. Repeatedly finds the most frequent pair of adjacent tokens.
3. Merges that pair into a new token.
4. Continues until a desired vocabulary size is reached.

BPE is widely used in machine learning for subword tokenization, especially in models like GPT, BERT, and other transformers.

---

## 🧾 What This Code Does

This project includes two main components:

- A **vocabulary generator** using multithreaded BPE (`tokenizer(...)`)
- A **text tokenizer** that applies the learned vocab to new text (`tokenize_file(...)`)

### 🔧 Train a BPE Vocabulary

Use the `tokenizer()` function to generate a vocabulary from a corpus. This function reads an input file in chunks (one per thread), merges the most common pairs using BPE, and stores the results in a shared dictionary.

```c
tokenizer(
    "../data/infiniteJest.txt",   // Input file
    10000,                        // Desired vocab size
    250000,                       // Bytes per thread
    "../data/myTokens.txt",       // Output vocab file
    8                             // Max number of threads
);
```

### ✂️ Tokenize New Text with Existing Vocab

Use `tokenize_file()` to apply a previously generated vocabulary to new text using greedy merging:

```c
tokenize_file(
    "../data/test.txt",           // File to tokenize
    "../data/myTokens.txt",       // Vocab file (format: id,token)
    10000                         // Vocabulary size
);
```

The vocab file should be in the format:

```
1,the
2,er
3,e
...
```

---

## ✨ Features

- 🔀 **Multithreaded** — Parallel vocab generation using POSIX threads
- 🧠 **Memory pool** — Custom allocator for fast, low-fragmentation memory reuse
- 📚 **BPE tokenization** — Works well for NLP, compression, and language modeling
- 🧮 **Configurable vocabulary size**
- 📦 **Adjustable chunk size per thread**
- 📜 **Greedy token merging** when applying vocab to new text

---

## ⚙️ Build Instructions

```bash
# Clone the repository
git clone https://github.com/phagmaier/Tokenizer.git
cd Tokenizer

# Create and move into the build directory
mkdir build && cd build

# Build the project
cmake ..
cmake --build .
```

✅ This will produce the `runme` executable inside `build/`.

---

## 🧪 Usage via main.c

> ⚠️ `main.c` is included for demonstration purposes. If you're using this in your own project, you can exclude `main.c` and just link the source files.

### CLI Flags

| Flag | Description           | Default                |
|------|-----------------------|------------------------|
| `-i` | Input file            | `../data/data.txt`     |
| `-o` | Output vocab file     | `../data/myTokens.txt` |
| `-v` | Vocabulary size       | `10000`                |
| `-b` | Bytes per thread      | `250000`               |
| `-t` | Max threads           | `15`                   |

### Examples

Default run:

```bash
./runme
```

Custom run:

```bash
./runme -i ../data/big.txt -o ../data/vocab.txt -v 8000 -b 500000 -t 8
```

---

## 📁 Project Structure

```
Tokenizer/
├── src/
│   ├── Dics.c/.h         # Dictionary, hashing, vocab counters
│   ├── Token.c/.h        # Token data structures and helpers
│   ├── Helper.c/.h       # Misc utility functions
│   ├── Tokenizer.c/.h    # BPE training logic
│   ├── TextToken.c/.h    # Applies learned vocab to new text
├── main.c                # Example usage (optional)
├── CMakeLists.txt        # Build configuration
└── data/                 # Sample input/output files
```

---

## ⚡ Benchmarks

- 📘 *Infinite Jest* (3.2MB): ~9 sec with 8 threads (default settings)
- 📦 167MB corpus: ~11 sec with 15 threads

---

## 📝 NOTE

💡 The larger the bytes per thread the larger the chunk to be tokenized will be leading to more "accurate" results.

---

## 🧠 Limitations

- ❌ Only supports ASCII input

---

## 🤝 Contributing

No formal guidelines — feel free to fork or PR improvements!

---

## 📫 Contact

**Parker Hagmaier**  
📧 [parkerhagmaier@gmail.com](mailto:parkerhagmaier@gmail.com)

---
