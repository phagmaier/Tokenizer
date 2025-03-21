# TOKENIZER

## DESCRIPTION
tokenizes text using BPE (Byte Pair Encoding) algo.
Currently it only processes a single hard coded file 
in the data folder. That being a text file of the book 
infinite jest. The goal is to put a bunch of text files 
in that folder and process them in batches. A simple and much 
slower algo exists in the testing folder written in python.
Just made it to check the accuracy of my approach.
The goal is to train the tokenizer using a NN written 
from scratch in C

## File Table

| File | Descripton |
| --------- | --------------------- |
| [Dic.c](src/Dic.c) | dictionary/map to get max token |
| [StrArr.h](src/StrArr.h) | dynamic array for tokens + mempools |
| [Dic.h](src/Dic.h) | dic/map header |
| [main.c](src/main.c) | runs test on the book Infinite Jest |
| [StrArr.c](src/StrArr.c) | header file for strings stringarr and mempool |


## TO-DO
1. Use threads 
2. Add support so it can load multiple txt files at a time to tokenize on larger data
3. Dynamic arrays don't need to be that dynamic. So don't need checks. The largest number of strings possible is the first pass
4. There are probably still a lot of performance improvements to make that I will address when writing the threaded version
5. Convert to a embedding matrix
6. Actually tokenize (give numeric values) put it through my from scratch NN/Transformer and train it 



## LONG TERM TO-DO
1. Rewrite my autograd class in C++
2. Write a performant NN/Transformer to train tokenizer 
3. Eventually expand this out to build my own little LLM



