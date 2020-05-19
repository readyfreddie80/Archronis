# Archronis

The program uses static Huffman`s coding for:
- compressing N files (N >= 1) to an archive
- decompressing an archive 

Vector, (inreasing priority) Queue, Tree and the nessesary operations on them are implemented from scratch 

# HOW TO USE

- Compressing mode

Put all the files you want to compress into the foulder with executable file. To run input the following command: 
```
 Archronis -c in-file1 [in-file2 ... in-fileN] new-archive-name
```
**in-file1 [in-file2 ... in-fileN]** - names of the files for compressing 

**new-archive-name** - the name of a new archive all the files will be packed into 

- Decompressing mode

Put the archive you want to unpack into the foulder with executable file. To run input the following command: 
```
 Archronis -d archive-to-decompress
```
**archive-to-decompress** - the name of the achive to unpack

# A few words about how the program works 

Files will be put in the archive in the order they are given to the program. Each symbol of the original alphabet should be encoded in one byte. 

First of all, to save the names of the files that are going to be compressed the program creates a so-called "info file", into which it writes the names one by one using "|" as a separator. This info file will be compressed as well and it will go first in the list of the files for compressing. The info file will be removed after that. 

Then, the program goes through the list of the files for compressing and gathers the common statistics on how frequently each symbol of the alphabet occurs. 

Based on these statistics, it builds the Huffman tree (that gives the length of codeword for each symbol) and then the table with the canonical Huffman code. Details: http://compression.ru/download/articles/huff/simakov_2002_huffcode.html 

When it comes to writing into the archive file, the information for decoding goes first. This information contains: 
1) The number of the encoded files  
2) The sizes of the files in the order of compressing 
3) The maximum length of the codewords 
4) The number of the codewords of each length (from 1 up to maximum length)
5) The symbols that are encoded in the order of increasing of the codeword lengths. For the symbols that are encoded by a codeword of the same length, the lexicographical order is used.

Finally, the program writes encoding for each file using the table with codewords. 

Basically, the decoding process uses the same steps but the other way around. 

(c) sklyarova.ev@phystech.edu, 2020
